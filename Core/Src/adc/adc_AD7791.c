/*
 * adc_AD7791.c
 *
 *  Created on: Dec 6, 2024
 *      Author: Kirill
 */

#include "adc_AD7791.h"
#include "malloc.h"
#include <string.h>
#include <math.h>

#define SPI_TIMEOUT 10

static void spi_select(adc_t* self);
static void spi_deselect(adc_t* self);
static void spi_hw_command(adc_t *self, uint8_t cmd);

struct adc_data_t
{
	SPI_HandleTypeDef* hspi;
	GPIO_TypeDef* portCS;
	uint16_t pinCS;
	uint8_t bitResolution;
	double Vref;
	int32_t lastOutputValue;
	int32_t maxOutputValue;

	uint8_t FR_word;
	uint8_t MR_word;
};

static void init(adc_t* self)
{
	spi_select(self);
	spi_hw_command(self, 0x20); // write to filter register
	spi_hw_command(self, self->data->FR_word);
	spi_deselect(self);
	HAL_Delay(10);


	spi_select(self);
	spi_hw_command(self, 0x10); // write to mode register
	spi_hw_command(self, self->data->MR_word);
	spi_deselect(self);
	HAL_Delay(10);
}

static void update(adc_t* self, void* option)
{
	const uint8_t kDataSizeBytes = 3;
	const uint8_t kBufferSizeBytes = 4;

	uint8_t rxBytes [kBufferSizeBytes];
	memset(rxBytes, 0, kBufferSizeBytes);

	spi_select(self);
	spi_hw_command(self, 0x38); // read data register
	int i;
	for(i = 0; i < kDataSizeBytes; ++i)
	{
		HAL_SPI_Receive(self->data->hspi, rxBytes + kDataSizeBytes - i - 1, 1, SPI_TIMEOUT);
		while(HAL_SPI_GetState(self->data->hspi) != HAL_SPI_STATE_READY)
			;
	}
	spi_deselect(self);
	self->data->lastOutputValue = *(int32_t*)rxBytes;
}

static uint32_t get_cnt(adc_t* self)
{
	return self->data->lastOutputValue;
}

static double get_vout(adc_t* self)
{
	return self->data->Vref * get_cnt(self) / self->data->maxOutputValue;
}

static struct adc_vtable methods = {init, update, get_cnt, get_vout};

adc_t adc_AD7791_create(
		SPI_HandleTypeDef* hspi,
		GPIO_TypeDef* portCS,
		uint16_t pinCS,
		double Vref,
		uint8_t FR_word,
		uint8_t MR_word
		)
{
	adc_t adc;
	adc.vtable = &methods;
	struct adc_data_t* pdata =
			(struct adc_data_t*)malloc(sizeof(struct adc_data_t));
	if(pdata)
	{
		memset(pdata, 0, sizeof(*pdata));
		pdata->hspi = hspi;
		pdata->portCS = portCS;
		pdata->pinCS = pinCS;
		pdata->Vref = Vref;
		pdata->lastOutputValue = 0;
		pdata->bitResolution = 24;
		pdata->maxOutputValue = (uint32_t)pow(2, pdata->bitResolution);
		pdata->FR_word = FR_word;
		pdata->MR_word = MR_word;
		adc.data = pdata;
	}
	// init
	return adc;
}

static void spi_select(adc_t* self)
{
	HAL_GPIO_WritePin(
			self->data->portCS,
			self->data->pinCS,
			GPIO_PIN_RESET
			);
}
static void spi_deselect(adc_t* self)
{
	HAL_GPIO_WritePin(
			self->data->portCS,
			self->data->pinCS,
			GPIO_PIN_SET
			);
}

static void spi_hw_command(adc_t *self, uint8_t cmd)
{
	HAL_SPI_Transmit(self->data->hspi, &cmd, 1, SPI_TIMEOUT);
	while(HAL_SPI_GetState(self->data->hspi) != HAL_SPI_STATE_READY)
		;
}

/*static void delay_us(volatile uint32_t us)
{
	int SYSTICK_LOAD = SystemCoreClock / 1000000U;
	int SYSTICK_DELAY_CALIB = SYSTICK_LOAD >> 1;
    do {
         uint32_t start = SysTick->VAL;
         uint32_t ticks = (us * SYSTICK_LOAD)-SYSTICK_DELAY_CALIB;
         while((start - SysTick->VAL) < ticks);
    } while (0);
} */




