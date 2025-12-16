/*
 * ADS1246.h
 *
 *  Created on: Dec 15, 2025
 *      Author: Kirill
 */

#ifndef SRC_ADC_ADS1246_H_
#define SRC_ADC_ADS1246_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef enum
{
	ADS1246_EMPTY,
	ADS1246_MEASURE_TEMP,
	ADS1246_MEASURE_AIN
} ADS1246_state_t;

typedef struct
{
	// configuration data
	SPI_HandleTypeDef* hspi;
	GPIO_TypeDef* portCS;
	uint16_t pinCS;
	GPIO_TypeDef* portXDRDY;
	uint16_t pinXDRDY;
	GPIO_TypeDef* portSTART;
	uint16_t pinSTART;
	GPIO_TypeDef* portRESET;
	uint16_t pinRESET;
	uint8_t bitResolution;
	double Vref;
	uint8_t SYS0_conf;

	// states
	ADS1246_state_t oldMeasureState;
	ADS1246_state_t newMeasureState;

	// measured values
	int32_t lastOutputValue;
	int32_t maxOutputValue;

	int32_t lastTemperatureRawValue;
	int32_t lastTemperatureMillicelcius;
} ADS1246_t;

// init
void ADS1246_init(ADS1246_t* self);
void ADS1246_set_reference_voltage(ADS1246_t* self, double Vref);
void ADS1246_set_spi(ADS1246_t* self, SPI_HandleTypeDef* hspi);
void ADS1246_set_cs_pin(ADS1246_t* self, GPIO_TypeDef* port, uint16_t pin);
void ADS1246_set_xdrdy_pin(ADS1246_t* self, GPIO_TypeDef* port, uint16_t pin);
void ADS1246_set_start_pin(ADS1246_t* self, GPIO_TypeDef* port, uint16_t pin);
void ADS1246_set_reset_pin(ADS1246_t* self, GPIO_TypeDef* port, uint16_t pin);

// operation mode
void ADS1246_switch_to_analog_input_mode(ADS1246_t* self);
void ADS1246_switch_to_temperature_mode(ADS1246_t* self);
ADS1246_state_t ADS1246_get_measure_mode(ADS1246_t* self);
void ADS1246_spi_setup(ADS1246_t* self);
void ADS1246_spi_update(ADS1246_t* self); // in xdrdy interrupt

// output
int32_t ADS1246_get_analog_input_value(ADS1246_t* self);
int32_t ADS1246_get_temperature_mcelcius(ADS1246_t* self);

// for detecting interrupt pin
uint16_t ADS1246_get_xdrdy_pin(ADS1246_t* self);

#endif /* SRC_ADC_ADS1246_H_ */
