/*
 * range_select.h
 *
 *  Created on: Dec 16, 2024
 *      Author: Kirill
 */

#ifndef SRC_ADC_RANGE_SELECT_H_
#define SRC_ADC_RANGE_SELECT_H_

#include "stm32f4xx_hal.h"

void set_adc_dose_range_select_pin(GPIO_TypeDef* port, uint16_t pin);
void select_broad_adc_dose_range();
void select_narrow_adc_dose_range();
int8_t get_current_adc_dose_range();

#endif /* SRC_ADC_RANGE_SELECT_H_ */
