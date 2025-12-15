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
static uint8_t CMD_SLEEP 	= 0x02;
static uint8_t CMD_SYNC 	= 0x04;
static uint8_t CMD_RESET 	= 0x04;
static uint8_t CMD_NOP 		= 0xFF;
static uint8_t CMD_RDATA 	= 0x12;
static uint8_t CMD_RDATAC 	= 0x14;
static uint8_t CMD_RREG 	= 0x20;
static uint8_t CMD_WREG 	= 0x40;
static uint8_t CMD_SYSOCAL 	= 0x60;
static uint8_t CMD_SYSGCAL 	= 0x61;
static uint8_t CMD_SELFOCAL = 0x62;
static uint8_t REG_SYS0 = 0x03;

static const uint16_t spiTimeout = 10;

static void ADS1246_spi_command(ADS1246_t* self, uint8_t cmd);
static void ADS1246_spi_select(ADS1246_t* self);
static void ADS1246_spi_deselect(ADS1246_t* self);
static void ADS1246_check_negative_24_to_32(int32_t* val);


static int ADS1246_raw_temperature_to_celcius(ADS1246_t* self, int rawTemp);
static void ADS1246_spi_setup_temperature_mode(ADS1246_t* self);
static void ADS1246_spi_setup_analog_input_mode(ADS1246_t* self);

void ADS1246_init(ADS1246_t* self)
{
	memset(self, 0, sizeof(*self));

	self->lastOutputValue = 0;
	self->bitResolution = 24;
	self->maxOutputValue = self->maxOutputValue = (uint32_t)pow(2, self->bitResolution);
	self->oldMeasureState = ADS1246_EMPTY;
	self->newMeasureState = ADS1246_MEASURE_AIN;
	self->Vref = 2.048;
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

void ADS1246_switch_to_analog_input_mode(ADS1246_t* self)
{
	self->newMeasureState = ADS1246_MEASURE_AIN;
}

void ADS1246_switch_to_temperature_mode(ADS1246_t* self)
{
	self->newMeasureState = ADS1246_MEASURE_TEMP;
}

ADS1246_t ADS1246_get_measure_mode(ADS1246_t* self)
{
	return self->oldMeasureState;
}

// private
static void ADS1246_spi_command(ADS1246_t* self, uint8_t cmd)
{
	if(self->hspi)
	{
		HAL_SPI_Transmit(self->hspi, &cmd, 1, spiTimeout);
		while(HAL_SPI_GetState(self->hspi) != HAL_SPI_STATE_READY);
	}
}

static void ADS1246_spi_select(ADS1246_t* self)
{
	if(self->portCS)
	{
		HAL_GPIO_WritePin(self->portCS, self->pinCS, GPIO_PIN_RESET);
	}
}

static void ADS1246_spi_deselect(ADS1246_t* self)
{
	if(self->portCS)
	{
		HAL_GPIO_WritePin(self->portCS, self->pinCS, GPIO_PIN_SET);
	}
}

static void ADS1246_check_negative_24_to_32(int32_t* val)
{
	if ((*val >> 23) & (int)1)
	{
		*val |= 0xFF000000;
	}
}

static int ADS1246_raw_temperature_to_celcius(ADS1246_t* self, int rawTemp)
{
	// T = 25 C, V = 118 mV, dV/dT = 405 uV/C
	return rawTemp; // debug!!!
}

static void ADS1246_spi_setup_temperature_mode(ADS1246_t* self)
{

}

static void ADS1246_spi_setup_analog_input_mode(ADS1246_t* self)
{

}











