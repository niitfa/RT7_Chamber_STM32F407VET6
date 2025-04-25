/*
 * pressure_sensor.h
 *
 *  Created on: Mar 5, 2025
 *      Author: Kirill
 */

#ifndef SRC_PRESSURE_SENSOR_PRESSURE_SENSOR_H_
#define SRC_PRESSURE_SENSOR_PRESSURE_SENSOR_H_

#include "adc.h"

typedef struct
{
	float pressureOffsetPa;
	float PaPerV;
	adc_t* adc;
} pressure_sensor_t;

int pressure_sensor_init(pressure_sensor_t* self, float pressureOffsetPa, float PaPerV, adc_t* adc);
float pressure_sensor_get_Pa(pressure_sensor_t* self);

#endif /* SRC_PRESSURE_SENSOR_PRESSURE_SENSOR_H_ */
