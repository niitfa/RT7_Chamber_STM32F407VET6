/*
 * adc_AD7791.h
 *
 *  Created on: Dec 6, 2024
 *      Author: Kirill
 */

#ifndef SRC_ADC_ADC_AD7791_H_
#define SRC_ADC_ADC_AD7791_H_

/* SPI CONF
 * freq < 5 MHz
 * CPOL = HIGH
 * CPHA = 2 (from 1 and 2)
 */

#include "adc.h"
#include "stm32f4xx_hal.h"



// heap allocation here!!!

adc_t adc_AD7791_create(
		SPI_HandleTypeDef* hspi,
		GPIO_TypeDef* portCS,
		uint16_t pinCS,
		double Vref,
		uint8_t FR_word,
		uint8_t MR_word,
		uint32_t waitCycles
		);

#endif /* SRC_ADC_ADC_AD7791_H_ */
