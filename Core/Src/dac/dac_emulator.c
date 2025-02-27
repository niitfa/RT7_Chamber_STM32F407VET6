/*
 * dac_emulator.c
 *
 *  Created on: Dec 16, 2024
 *      Author: Kirill
 */

#include "dac_emulator.h"
#include "malloc.h"
#include <string.h>
#include <math.h>

struct dac_data_t
{
	uint8_t bitResolution;
	double Vref;
	uint32_t maxInputValue;
};

static void init(dac_t* self)
{
	self->data->Vref = 2.048;
}

static void set_digital_value(dac_t* self, uint32_t digital_value)
{
}

static double get_reference_voltage(dac_t* self)
{
	return self->data->Vref;
}

static struct dac_vtable methods = {init, set_digital_value, get_reference_voltage};

dac_t dac_emulator_create()
{
	dac_t dac;
	dac.vtable = &methods;
	struct dac_data_t* pdata =
			(struct dac_data_t*)malloc(sizeof(struct dac_data_t));
	if(pdata)
	{
		memset(pdata, 0, sizeof(*pdata));
		// fill data structure
		pdata->bitResolution = 10;
		pdata->maxInputValue = (uint32_t)pow(2, pdata->bitResolution);
		dac.data = pdata;
	}
	return dac;
}
