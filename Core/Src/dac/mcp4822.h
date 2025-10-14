/*
 * mcp4822.h
 *
 *  Created on: Aug 12, 2025
 *      Author: Kirill
 */

#ifndef SRC_DAC_MCP4822_H_
#define SRC_DAC_MCP4822_H_

/*
 * 1) SPI MODE = 0:0, 1:1
 * 2) SPI FREQ < 20 MHz
 * 3) MSB first
 * 4) LDAC can be tied to low (VSS) if the VOUT update is desired at the
 * rising edge of the CS pin
 */

#include "stm32f4xx_hal.h"

typedef struct
{
	SPI_HandleTypeDef* hspi;
	GPIO_TypeDef* portCS;
	uint16_t pinCS;
	GPIO_TypeDef* portLDAC;
	uint16_t pinLDAC;
	uint8_t bitResolution;
	double Vref;
	uint32_t maxInputValue;
	uint8_t gain;
	uint8_t channel;
} mcp4822_t;

void mcp4822_init(
		mcp4822_t* self,
		SPI_HandleTypeDef* hspi,
		GPIO_TypeDef* portCS,
		uint16_t pinCS,
		GPIO_TypeDef* portLDAC,
		uint16_t pinLDAC
		);

void mcp4822_set_input_value(mcp4822_t* self, uint16_t digital_value, uint16_t channel);
void mcp4822_shutdown(mcp4822_t* self);

#endif /* SRC_DAC_MCP4822_H_ */
