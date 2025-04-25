/*
 * pressure_sensor.c
 *
 *  Created on: Mar 5, 2025
 *      Author: Kirill
 */
#include <string.h>

#include "pressure_sensor.h"

int pressure_sensor_init(pressure_sensor_t* self, float pressureOffsetPa, float PaPerV, adc_t* adc)
{
	memset(self, 0, sizeof(*self));
	self->pressureOffsetPa = pressureOffsetPa;
	self->PaPerV = PaPerV;
	self->adc = adc;
	return 0;
}

float pressure_sensor_get_Pa(pressure_sensor_t* self)
{
	return adc_get_vout(self->adc) * self->PaPerV - self->pressureOffsetPa;
}


