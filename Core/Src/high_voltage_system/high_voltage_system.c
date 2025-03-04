/*
 * hv_source.c
 *
 *  Created on: Dec 16, 2024
 *      Author: Kirill
 */
#include "high_voltage_system.h"
#include <string.h>
#include <math.h>

static const int HV_POSITIVE_SOURCE = 0;
static const int HV_NEGATIVE_SOURCE = 1;

void hv_init(high_voltage_system_t* self,
		dac_t* inputDAC,
		adc_t* outputADC,
		GPIO_TypeDef* portSelectHV,
		uint16_t pinSelectHV,
		double VoutStep_V,
		double VoutGainADC,
		double VoutMax
		)
{
	memset(self, 0, sizeof(*self));
	self->inputDAC 		= inputDAC;
	self->outputADC 	= outputADC;
	self->portSelectHV 	= portSelectHV;
	self->pinSelectHV 	= pinSelectHV;
	self->VoutStep_V 	= VoutStep_V;
	self->VoutGainADC 	= VoutGainADC;
	self->VoutMax		= VoutMax;

	hv_select_positive_source(self);
	hv_set_output_voltage_adc_offset(self, 0);
}

void hv_select_negative_source(high_voltage_system_t* self)
{
	HAL_GPIO_WritePin(self->portSelectHV, self->pinSelectHV, HV_NEGATIVE_SOURCE);
}

void hv_select_positive_source(high_voltage_system_t* self)
{
	HAL_GPIO_WritePin(self->portSelectHV, self->pinSelectHV, HV_POSITIVE_SOURCE);
}

void hv_set_abs_output_voltage_V(high_voltage_system_t* self, double Vout)
{
	uint32_t digital = (uint32_t)round(fmin(Vout, self->VoutMax) / self->VoutStep_V);
	dac_set_input_value(self->inputDAC, digital);
}

double hv_get_output_voltage_V(high_voltage_system_t* self)
{
	return (adc_get_vout(self->outputADC) - self->VoutOffsetADC_V) / self->VoutGainADC;
}

void hv_set_output_voltage_adc_offset(high_voltage_system_t* self, double offset_V)
{
	self->VoutOffsetADC_V = offset_V;
}
