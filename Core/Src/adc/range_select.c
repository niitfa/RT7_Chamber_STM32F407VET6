/*
 * range_select.c
 *
 *  Created on: Dec 16, 2024
 *      Author: Kirill
 */

#include "range_select.h"

static GPIO_TypeDef* rangeSelectPort = NULL;
static uint16_t rangeSelectPin = 0;

static const int BROAD_RANGE_PIN_STATE = 0;
static const int NARROW_RANGE_PIN_STATE = 1;

void set_adc_dose_range_select_pin(GPIO_TypeDef* port, uint16_t pin)
{
	rangeSelectPort = port;
	rangeSelectPin = pin;
}

void select_broad_adc_dose_range()
{
	if(rangeSelectPort)
	{
		HAL_GPIO_WritePin(rangeSelectPort, rangeSelectPin, BROAD_RANGE_PIN_STATE);
	}
}

void select_narrow_adc_dose_range()
{
	if(rangeSelectPort)
	{
		HAL_GPIO_WritePin(rangeSelectPort, rangeSelectPin, NARROW_RANGE_PIN_STATE);
	}
}


