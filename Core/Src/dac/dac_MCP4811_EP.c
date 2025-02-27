/*
 * dac_MCP4811_EP.c
 *
 *  Created on: Dec 16, 2024
 *      Author: Kirill
 */
#include "dac_MCP4811_EP.h"
#include "malloc.h"
#include <string.h>
#include <math.h>

#define SPI_TIMEOUT 10

static void spi_hw_command(dac_t *self, uint8_t cmd);
static void spi_select(dac_t* self);
static void spi_deselect(dac_t* self);

static void init(dac_t* self);
static void set_digital_value(dac_t* self, uint32_t digital_value);
static double get_reference_voltage(dac_t* self);

struct dac_data_t
{
	SPI_HandleTypeDef* hspi;

	GPIO_TypeDef* portCS;
	uint16_t pinCS;
	GPIO_TypeDef* portLDAC;
	uint16_t pinLDAC;
	GPIO_TypeDef* portSHDW;
	uint16_t pinSHDW;

	uint8_t bitResolution;
	double Vref;
	uint32_t maxInputValue;
};

static void init(dac_t* self)
{
	spi_deselect(self);
	HAL_Delay(10);

	if(self->data->portLDAC)
	{
		HAL_GPIO_WritePin(self->data->portLDAC, self->data->pinLDAC, 0);
	}

	if(self->data->portSHDW)
	{
		HAL_GPIO_WritePin(self->data->portSHDW, self->data->pinSHDW, 1);
	}

	// SPI config begin
	//spi_select(self);
	//spi_deselect(self);

	// SPI config end
	self->data->Vref = 2.048;

	// always send zero after init
	set_digital_value(self, 0);
}

static void set_digital_value(dac_t* self, uint32_t digital_value)
{
	if(digital_value < self->data->maxInputValue)
	{
	    uint16_t shdw = 1;
	    uint16_t gain = 1;
	    uint16_t x = digital_value;
	    uint16_t val = 0;
	    val += (shdw <<= 12);
	    val += (gain <<= 13);
	    val += (x <<= 2);

		// SPI Transmit digital value begin
		spi_select(self);
		//spi_hw_command(self, (uint8_t)val);
		spi_hw_command(self, (uint8_t)(val >> 8));
		spi_hw_command(self, (uint8_t)val);
		spi_deselect(self);
		// SPI Transmit digital value end

	}
}

static double get_reference_voltage(dac_t* self)
{
	return self->data->Vref;
}

static struct dac_vtable methods = {init, set_digital_value, get_reference_voltage};

dac_t dac_MCP4811EP_create(
		SPI_HandleTypeDef* hspi,
		GPIO_TypeDef* portCS,
		uint16_t pinCS,
		GPIO_TypeDef* portLDAC,
		uint16_t pinLDAC,
		GPIO_TypeDef* portSHDW,
		uint16_t pinSHDW
		)
{
	dac_t dac;
	dac.vtable = &methods;
	struct dac_data_t* pdata =
			(struct dac_data_t*)malloc(sizeof(struct dac_data_t));
	if(pdata)
	{
		memset(pdata, 0, sizeof(*pdata));
		// fill data structure
		pdata->hspi = hspi;
		pdata->portCS = portCS;
		pdata->pinCS = pinCS;
		pdata->portLDAC = portLDAC;
		pdata->pinLDAC = pinLDAC;
		pdata->portSHDW = portSHDW;
		pdata->pinSHDW = pinSHDW;
		pdata->bitResolution = 10;
		pdata->maxInputValue = (uint32_t)pow(2, pdata->bitResolution);
		dac.data = pdata;
	}
	return dac;
}

static void spi_select(dac_t* self)
{
	HAL_GPIO_WritePin(
			self->data->portCS,
			self->data->pinCS,
			GPIO_PIN_RESET
			);
}

static void spi_deselect(dac_t* self)
{
	HAL_GPIO_WritePin(
			self->data->portCS,
			self->data->pinCS,
			GPIO_PIN_SET
			);
}

static void spi_hw_command(dac_t *self, uint8_t cmd)
{
	HAL_SPI_Transmit(self->data->hspi, &cmd, 1, SPI_TIMEOUT);
	while(HAL_SPI_GetState(self->data->hspi) != HAL_SPI_STATE_READY)
		;
}

