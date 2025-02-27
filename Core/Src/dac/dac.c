/*
 * dac.c
 *
 *  Created on: Dec 16, 2024
 *      Author: Kirill
 */

#include "dac.h"

void dac_init(dac_t* self)
{
	self->vtable->init(self);
}

void dac_set_input_value(dac_t* self, uint32_t digital_value)
{
	self->vtable->set_input_value(self, digital_value);
}

double dac_get_reference_voltage(dac_t* self)
{
	return self->vtable->get_reference_voltage(self);
}


