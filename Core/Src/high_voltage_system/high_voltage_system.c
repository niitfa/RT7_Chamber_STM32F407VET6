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
		//dac_t* inputDAC,
		mcp4822_t* inputDAC,
		adc_t* outputADC,
		GPIO_TypeDef* portSelectHV,
		uint16_t pinSelectHV,
		double HVOutputToADCInputGain,
		double dacOutputToHVInputGain,
		double HVInputToHVOutputGain
		)
{
	memset(self, 0, sizeof(*self));
	self->inputDAC 		= inputDAC;
	self->outputADC 	= outputADC;
	self->portSelectHV 	= portSelectHV;
	self->pinSelectHV 	= pinSelectHV;
	self->HVOutputToADCInputGain 	= HVOutputToADCInputGain;
	self->dacOutputToHVInputGain = dacOutputToHVInputGain;
	self->HVInputToHVOutputGain  = HVInputToHVOutputGain;
	self->VoutOffsetADC = 0;


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

int8_t hv_get_source_polarity(high_voltage_system_t* self)
{
	return HAL_GPIO_ReadPin(self->portSelectHV, self->pinSelectHV);
}

void hv_set_abs_output_voltage_V(high_voltage_system_t* self, double Vout)
{
	// 1 set target output voltage Vout
	// 2 rescale it to DAC input
	double HVInput_V = Vout / self->HVInputToHVOutputGain;
	double DACOutput_V = HVInput_V / self->dacOutputToHVInputGain;

	double DACInputRaw = (double)mcp4822_get_max_input_value(self->inputDAC) *
			(DACOutput_V / mcp4822_get_reference_voltage(self->inputDAC));
	uint32_t DACInput = (uint32_t)round(DACInputRaw);

	// 3 check if limits are exceeded
	// filter maximum
	DACInput = (DACInput < mcp4822_get_max_input_value(self->inputDAC) - 1) ?
			DACInput : (mcp4822_get_max_input_value(self->inputDAC) - 1);

	// filter minimum
	DACInput = (DACInput >= 0) ? DACInput : 0;

	// 4 send final value
	mcp4822_set_input_value(self->inputDAC, DACInput, 0);
}

int16_t hv_get_output_voltage_V(high_voltage_system_t* self)
{
	return (adc_get_vout(self->outputADC) - self->VoutOffsetADC) / self->HVOutputToADCInputGain;
	//int32_t adcOutput = adc_get_cnt(self->outputADC);

	//double adcInputVoltage = adc_get_vout(self)(self->outputADC);
	//double adcInputVoltageShifted = adcInputVoltage - self->VoutOffsetADC;
	//int16_t HVInput = adcInputVoltageShifted / self->HVOutputToADCInputGain;
}

void hv_set_output_voltage_adc_offset(high_voltage_system_t* self, double offset_V)
{
	self->VoutOffsetADC = offset_V;
}
