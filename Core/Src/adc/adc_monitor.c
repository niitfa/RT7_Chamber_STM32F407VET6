/*
 * adc_monitor.c
 *
 *  Created on: Dec 10, 2024
 *      Author: Kirill
 */

#include "adc_monitor.h"
#include <string.h>
#include <math.h>


void adc_monitor_init(adc_monitor_t* self, adc_t* adc, IRQn_Type IRQn)
{
	memset(self, 0, sizeof(*self));
	self->adc = adc;
	self->IRQn = IRQn;
	self->state = ADC_STANDBY;
}

void adc_monitor_update(adc_monitor_t* self)
{
	self->currValue = adc_get_vout(self->adc);
	switch(self->state)
	{
	case ADC_STANDBY:
		self->averageValue 			= 0;
		self->measurementCycles 	= 0;
		self->measurementCyclesMax	= 0;
		break;
	case ADC_PROCESS:
		self->measurementCycles++;
		self->averageValue =
				((double)self->measurementCycles - 1) / self->measurementCycles * self->averageValue +
				self->currValue / self->measurementCycles;
		if(self->measurementCycles >= self->measurementCyclesMax)
		{
			self->state = ADC_COMPLETED;
		}
		break;
	case ADC_COMPLETED:
		break;
	}
}

void adc_monitor_start_measurement(adc_monitor_t* self, uint32_t cycles)
{
	HAL_NVIC_DisableIRQ(self->IRQn);
	if(self->state == ADC_STANDBY)
	{
		self->measurementCycles = 0;
		self->measurementCyclesMax = cycles;
		self->state = ADC_PROCESS;
	}
	HAL_NVIC_EnableIRQ(self->IRQn);
}

void adc_monitor_reset_measurement(adc_monitor_t* self)
{
	HAL_NVIC_DisableIRQ(self->IRQn);
	self->state = ADC_STANDBY;
	HAL_NVIC_EnableIRQ(self->IRQn);
}

double adc_monitor_get_average_signal_value(adc_monitor_t* self)
{
	return self->averageValue;
}

uint32_t adc_monitor_get_measurement_cycle_no(adc_monitor_t* self)
{
	return self->measurementCycles;
}

int8_t adc_monitor_get_measurement_state(adc_monitor_t* self)
{
	return self->state;
}
