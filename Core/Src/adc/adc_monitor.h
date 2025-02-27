/*
 * adc_monitor.h
 *
 *  Created on: Dec 10, 2024
 *      Author: Kirill
 */

#ifndef SRC_ADC_ADC_MONITOR_H_
#define SRC_ADC_ADC_MONITOR_H_

#include "adc.h"
#include "stm32f4xx_hal.h"

typedef enum
{
	ADC_STANDBY 	= 0,
	ADC_PROCESS 	= 1,
	ADC_COMPLETED 	= 2
} adc_monitor_state_t;

typedef struct
{
	adc_t* adc;
	double currValue, averageValue;
	adc_monitor_state_t state;
	uint32_t measurementCycles, measurementCyclesMax;
	IRQn_Type IRQn;
} adc_monitor_t;

void adc_monitor_init(adc_monitor_t* self, adc_t* adc, IRQn_Type IRQn);
void adc_monitor_update(adc_monitor_t* self);

void adc_monitor_start_measurement(adc_monitor_t* self, uint32_t cycles);
void adc_monitor_reset_measurement(adc_monitor_t* self);

double adc_monitor_get_average_signal_value(adc_monitor_t* self);
uint32_t adc_monitor_get_measurement_cycle_no(adc_monitor_t* self);
int8_t adc_monitor_get_measurement_state(adc_monitor_t* self);


#endif /* SRC_ADC_ADC_MONITOR_H_ */
