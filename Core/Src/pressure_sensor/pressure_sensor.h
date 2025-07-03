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
	int pressureOffsetkPa;
	double kPaPerV;
	adc_t* adc;
} pressure_sensor_t;

int pressure_sensor_init(pressure_sensor_t* self, int pressureOffsetkPa, double kPaPerV, adc_t* adc);
int pressure_sensor_get_kPa(pressure_sensor_t* self);

#endif /* SRC_PRESSURE_SENSOR_PRESSURE_SENSOR_H_ */
