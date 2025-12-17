/*
 * ADS1246.c
 *
 *  Created on: Dec 15, 2025
 *      Author: Kirill
 */

#include "ADS1246.h"
#include <malloc.h>
#include <string.h>
#include <math.h>

static uint8_t CMD_WAKEUP 	= 0x00;
static uint8_t CMD_NOP 		= 0xFF;
static uint8_t CMD_RDATA 	= 0x12;
static uint8_t CMD_WREG 	= 0x40;
static uint8_t REG_SYS0 = 0x03;
static uint8_t REG_MUX = 0x02;
// MUX REGISTER
static uint8_t MUX_CLKSTAT_EXT = 0x80;
static uint8_t MUX_MUXCAL_DEFAULT = 0x00;
static uint8_t MUX_MUXCAL_TEMP = 0x03;

static const uint16_t spiTimeout = 10;

static inline void delay_us(volatile uint32_t us)
{
	us *= (SystemCoreClock / 1000000);
	while (us--);
}

static void spi_command(ADS1246_t* self, uint8_t cmd);
static void spi_select(ADS1246_t* self);
static void spi_deselect(ADS1246_t* self);

static void check_negative_24_to_32(int32_t* val);
static double ADS1246_raw_temperature_to_celcius(ADS1246_t* self, int rawTemp);

static void ADS1246_reset(ADS1246_t* self);
static int ADS1246_spi_read(ADS1246_t* self);
static void ADS1246_spi_wake_up(ADS1246_t* self);
static void ADS1246_spi_setup_sys0_reg(ADS1246_t* self);
static void ADS1246_spi_setup_new_mode(ADS1246_t* self);
static void ADS1246_spi_setup_temperature_mode(ADS1246_t* self);
static void ADS1246_spi_setup_analog_input_mode(ADS1246_t* self);

void ADS1246_init(ADS1246_t* self)
{
	memset(self, 0, sizeof(*self));

	self->lastOutputValue = 0;
	self->bitResolution = 24;
	self->maxOutputValue = (uint32_t)pow(2, self->bitResolution);
	self->oldMeasureState = ADS1246_EMPTY;
	self->newMeasureState = ADS1246_MEASURE_AIN;
	self->Vref = 2.048;
	self->sys0 = 0b00000010;
}

void ADS1246_set_reference_voltage(ADS1246_t* self, double Vref)
{
	self->Vref = Vref;
}

void ADS1246_set_spi(ADS1246_t* self, SPI_HandleTypeDef* hspi)
{
	self->hspi = hspi;
}

void ADS1246_set_cs_pin(ADS1246_t* self, GPIO_TypeDef* port, uint16_t pin)
{
	self->portCS = port;
	self->pinCS = pin;
}

void ADS1246_set_xdrdy_pin(ADS1246_t* self, GPIO_TypeDef* port, uint16_t pin)
{
	self->portXDRDY = port;
	self->pinXDRDY = pin;
}

void ADS1246_set_start_pin(ADS1246_t* self, GPIO_TypeDef* port, uint16_t pin)
{
	self->portSTART = port;
	self->pinSTART = pin;
}

void ADS1246_set_reset_pin(ADS1246_t* self, GPIO_TypeDef* port, uint16_t pin)
{
	self->portRESET = port;
	self->pinRESET = pin;
}

void ADS1246_set_sys0_reg(ADS1246_t* self, uint8_t sys0)
{
	self->sys0 = sys0;
}

void ADS1246_switch_to_analog_input_mode(ADS1246_t* self)
{
	self->newMeasureState = ADS1246_MEASURE_AIN;
}

void ADS1246_switch_to_temperature_mode(ADS1246_t* self)
{
	self->newMeasureState = ADS1246_MEASURE_TEMP;
}

ADS1246_state_t ADS1246_get_measure_mode(ADS1246_t* self)
{
	return self->oldMeasureState;
}

void ADS1246_spi_setup(ADS1246_t* self)
{
	ADS1246_reset(self);
	delay_us(10);
	ADS1246_spi_wake_up(self);
	delay_us(10);
	ADS1246_spi_setup_sys0_reg(self);
	delay_us(10);
	ADS1246_spi_setup_new_mode(self);
}

void ADS1246_spi_update(ADS1246_t* self)
{
	// get value spi
	int val = ADS1246_spi_read(self);
	// value interpretation depends on mode
	switch(self->oldMeasureState)
	{
	case ADS1246_MEASURE_TEMP:
		self->lastTemperatureRawValue = val;
		self->lastTemperatureMillicelcius = (int)(ADS1246_raw_temperature_to_celcius(self, val) * 100.);
		break;
	case ADS1246_MEASURE_AIN:
		self->lastOutputValue = val;
		break;
	default:
		break;
	}
	// next state if changed
	ADS1246_spi_setup_new_mode(self);
}

// private
static void spi_command(ADS1246_t* self, uint8_t cmd)
{
	if(self->hspi)
	{
		HAL_SPI_Transmit(self->hspi, &cmd, 1, spiTimeout);
		while(HAL_SPI_GetState(self->hspi) != HAL_SPI_STATE_READY);
	}
}

static void spi_select(ADS1246_t* self)
{
	if(self->portCS)
	{
		HAL_GPIO_WritePin(self->portCS, self->pinCS, GPIO_PIN_RESET);
	}
}

static void spi_deselect(ADS1246_t* self)
{
	if(self->portCS)
	{
		HAL_GPIO_WritePin(self->portCS, self->pinCS, GPIO_PIN_SET);
	}
}

static void check_negative_24_to_32(int32_t* val)
{
	if ((*val >> 23) & (int)1)
	{
		*val |= 0xFF000000;
	}
}

static void ADS1246_reset(ADS1246_t* self)
{
	spi_deselect(self);
	delay_us(1);
	if(self->portSTART) { HAL_GPIO_WritePin(self->portSTART, self->pinSTART, GPIO_PIN_RESET); }
	if(self->portRESET) { HAL_GPIO_WritePin(self->portRESET, self->pinRESET, GPIO_PIN_RESET); }
	delay_us(10);
	if(self->portSTART) { HAL_GPIO_WritePin(self->portSTART, self->pinSTART, GPIO_PIN_SET); }
	if(self->portRESET) { HAL_GPIO_WritePin(self->portRESET, self->pinRESET, GPIO_PIN_SET); }
}

static int ADS1246_spi_read(ADS1246_t* self)
{
	const uint8_t kDataSizeBytes = 3;
	const uint8_t kBufferSizeBytes = 4;
	uint8_t rxBytes [kBufferSizeBytes];

	memset(rxBytes, 0, kBufferSizeBytes);
	spi_select(self);
	spi_command(self, CMD_RDATA);
	int i;
	for(i = 0; i < kDataSizeBytes; ++i)
	{
		HAL_SPI_TransmitReceive(self->hspi, &CMD_NOP, rxBytes + kDataSizeBytes - i - 1, 1, spiTimeout);
		while(HAL_SPI_GetState(self->hspi) != HAL_SPI_STATE_READY)
			;
	}
	spi_deselect(self);
	check_negative_24_to_32((int32_t*)rxBytes);
	return *(int32_t*)rxBytes;

}

static void ADS1246_spi_wake_up(ADS1246_t* self)
{
	spi_select(self);
	spi_command(self, CMD_WAKEUP);
	spi_deselect(self);
}

static void ADS1246_spi_setup_sys0_reg(ADS1246_t* self)
{
	spi_select(self);
	spi_command(self, CMD_WREG | REG_SYS0);
	spi_command(self, 0); // 1 byte
	spi_command(self, self->sys0);
	spi_deselect(self);
}


static void ADS1246_spi_setup_temperature_mode(ADS1246_t* self)
{
	spi_select(self);
	spi_command(self, CMD_WREG | REG_MUX);
	spi_command(self, 0); // 1 byte
	spi_command(self, MUX_CLKSTAT_EXT | MUX_MUXCAL_TEMP);
	spi_deselect(self);
}

static void ADS1246_spi_setup_analog_input_mode(ADS1246_t* self)
{
	spi_select(self);
	spi_command(self, CMD_WREG | REG_MUX);
	spi_command(self, 0); // 1 byte
	spi_command(self, MUX_CLKSTAT_EXT | MUX_MUXCAL_DEFAULT);
}

static void ADS1246_spi_setup_new_mode(ADS1246_t* self)
{
	if(self->newMeasureState != self->oldMeasureState)
	{
		switch(self->newMeasureState) {
		case ADS1246_MEASURE_AIN:
			ADS1246_spi_setup_temperature_mode(self);
			break;
		case ADS1246_MEASURE_TEMP:
			ADS1246_spi_setup_analog_input_mode(self);
			break;
		default:
			break;
		}
	}
	self->oldMeasureState = self->newMeasureState;
}


static double ADS1246_raw_temperature_to_celcius(ADS1246_t* self, int rawTemp)
{
	double V_in = (double)rawTemp / (0.5 * self->maxOutputValue) * self->Vref;
	return 25. + (V_in - 0.118) * 2469.; // 1e+6 / 405 = 2469 C/V
}













