/*
 * dac.h
 *
 *  Created on: Dec 16, 2024
 *      Author: Kirill
 */

#ifndef SRC_DAC_DAC_H_
#define SRC_DAC_DAC_H_

#include <stdint.h>

struct dac_data_t;

typedef struct
{
	struct dac_vtable* vtable;
	struct dac_data_t* data;
} dac_t;

struct dac_vtable
{
	void (*init)(dac_t* self);
	void (*set_input_value)(dac_t* self, uint32_t digital_value);
	double (*get_reference_voltage)(dac_t* self);
};

void dac_init(dac_t* self);
void dac_set_input_value(dac_t* self, uint32_t digital_value);
double dac_get_reference_voltage(dac_t* self);

#endif /* SRC_DAC_DAC_H_ */
