/*
 * dac_MCP4811_EP.h
 *
 *  Created on: Dec 16, 2024
 *      Author: Kirill
 */

#ifndef SRC_DAC_DAC_MCP4811_EP_H_
#define SRC_DAC_DAC_MCP4811_EP_H_

#include "dac.h"
#include "stm32f4xx_hal.h"

dac_t dac_MCP4811EP_create(
		SPI_HandleTypeDef* hspi,
		GPIO_TypeDef* portCS,
		uint16_t pinCS,
		GPIO_TypeDef* portLDAC,
		uint16_t pinLDAC,
		GPIO_TypeDef* portSHDW,
		uint16_t pinSHDW
		);

#endif /* SRC_DAC_DAC_MCP4811_EP_H_ */
