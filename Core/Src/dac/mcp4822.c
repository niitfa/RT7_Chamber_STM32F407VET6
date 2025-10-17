/*
 * mcp4822.c
 *
 *  Created on: Aug 12, 2025
 *      Author: Kirill
 */

#include "mcp4822.h"
#include <string.h>
#include <math.h>

#define SPI_TIMEOUT 10

static void spi_hw_command(mcp4822_t *self, uint8_t cmd);
static void spi_select(mcp4822_t* self);
static void spi_deselect(mcp4822_t* self);

static void mcp4822_transmit(mcp4822_t* self, uint16_t digital_value, uint16_t shutdown, uint16_t gain, uint16_t channel);

void mcp4822_init(
		mcp4822_t* self,
		SPI_HandleTypeDef* hspi,
		GPIO_TypeDef* portCS,
		uint16_t pinCS,
		GPIO_TypeDef* portLDAC,
		uint16_t pinLDAC
		)
{
	memset(self, 0 , sizeof(*self));
	self->hspi = hspi;
	self->portCS = portCS;
	self->pinCS = pinCS;
	self->portLDAC = portLDAC;
	self->pinLDAC = pinLDAC;
	self->bitResolution = 12;
	self->maxInputValue = (uint32_t)pow(2, self->bitResolution);
	self->Vref = 2.048;
	self->gain = 1;
	self->channel = 0;

	spi_deselect(self);
}


void mcp4822_set_input_value(mcp4822_t* self, uint16_t digital_value, uint16_t channel)
{
	mcp4822_transmit(self, digital_value, 1, 1, channel);
}

void mcp4822_shutdown(mcp4822_t* self)
{
	mcp4822_transmit(self, 0, 0, 1, 0);
	HAL_Delay(1);
	mcp4822_transmit(self, 0, 0, 1, 1);
	HAL_Delay(1);
}

uint32_t mcp4822_get_max_input_value(mcp4822_t* self)
{
	return self->maxInputValue;
}

double mcp4822_get_reference_voltage(mcp4822_t* self)
{
	return self->Vref;
}

static void mcp4822_transmit(mcp4822_t* self, uint16_t digital_value, uint16_t shutdown, uint16_t gain, uint16_t channel)
{
	digital_value = (digital_value < self->maxInputValue) ? digital_value : (self->maxInputValue - 1);
	shutdown = shutdown ? 1 : 0;
	gain = gain ? 1 : 0;
    channel = channel ? 1 : 0;

    uint16_t val = 0;
	val += (channel <<= 15);
	val += (gain <<= 13);
	val += (shutdown <<= 12);
	val += (digital_value <<= 0);

	spi_select(self);
	spi_hw_command(self, (uint8_t)(val >> 8));
	spi_hw_command(self, (uint8_t)val);
	spi_deselect(self);
}


static void spi_select(mcp4822_t* self)
{
	if(self->portCS)
	{
		HAL_GPIO_WritePin(
				self->portCS,
				self->pinCS,
				GPIO_PIN_RESET
				);
	}
}

static void spi_deselect(mcp4822_t* self)
{
	if(self->pinCS)
	{
		HAL_GPIO_WritePin(
				self->portCS,
				self->pinCS,
				GPIO_PIN_SET
				);
	}
}

static void spi_hw_command(mcp4822_t *self, uint8_t cmd)
{
	if(self->hspi)
	{
		HAL_SPI_Transmit(self->hspi, &cmd, 1, SPI_TIMEOUT);
		while(HAL_SPI_GetState(self->hspi) != HAL_SPI_STATE_READY)
			;
	}
}
