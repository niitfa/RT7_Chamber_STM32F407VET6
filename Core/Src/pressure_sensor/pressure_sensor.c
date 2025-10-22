/*
 * pressure_sensor.c
 *
 *  Created on: Mar 5, 2025
 *      Author: Kirill
 */
#include <string.h>
#include <math.h>

#include "pressure_sensor.h"

int pressure_sensor_init(pressure_sensor_t* self, int pressureOffsetkPa, double kPaPerV, adc_t* adc)
{
	memset(self, 0, sizeof(*self));
	self->pressureOffsetkPa = pressureOffsetkPa;
	self->kPaPerV = kPaPerV;
	self->adc = adc;
	return 0;
}

int pressure_sensor_get_kPa(pressure_sensor_t* self)
{
	return (int)round((adc_get_vout(self->adc) * self->kPaPerV) + self->pressureOffsetkPa);
}


