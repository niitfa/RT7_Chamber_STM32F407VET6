/*
 * hv_source.h
 *
 *  Created on: Dec 16, 2024
 *      Author: Kirill
 */

#ifndef SRC_HIGH_VOLTAGE_SYSTEM_HIGH_VOLTAGE_SYSTEM_H_
#define SRC_HIGH_VOLTAGE_SYSTEM_HIGH_VOLTAGE_SYSTEM_H_

#include "dac.h"
#include "adc.h"

#include "stm32f4xx_hal.h"

/* TO DO
 * 1) V_out gain in ADC input (VoutGainADC)
 * 2) Pull down HV select pin
*/

typedef struct
{
	dac_t* inputDAC;
	adc_t* outputADC;
	GPIO_TypeDef* portSelectHV;
	uint16_t pinSelectHV;
	double VoutStep_V; // 1 V
	double VoutOffsetADC_V;
	double VoutGainADC;
	double VoutMax;
} high_voltage_system_t;

void hv_init(high_voltage_system_t* self,
		dac_t* inputDAC,
		adc_t* outputADC,
		GPIO_TypeDef* portSelectHV,
		uint16_t pinSelectHV,
		double VoutStep_V,
		double VoutGainADC,
		double VoutMax
		); // arguments list

void hv_select_negative_source(high_voltage_system_t* self);
void hv_select_positive_source(high_voltage_system_t* self);

void hv_set_abs_output_voltage_V(high_voltage_system_t* self, double Vout);
double hv_get_output_voltage_V(high_voltage_system_t* self);

void hv_set_output_voltage_adc_offset(high_voltage_system_t* self, double offset_V);

#endif /* SRC_HIGH_VOLTAGE_SYSTEM_HIGH_VOLTAGE_SYSTEM_H_ */
