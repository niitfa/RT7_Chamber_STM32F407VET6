/*
 * tx_message.c
 *
 *  Created on: Aug 16, 2024
 *      Author: Kirill
 */

#include "tx_message.h"
#include <string.h>

#define BYTE_POS_ID			 		0
#define BYTE_POS_ADC_DR				4
#define BYTE_POS_ADC_DR_AV			8

#define BYTE_POS_HV_OUT	 			12
#define BYTE_POS_HV_POL 			14
#define BYTE_POS_RANGE	 			15

#define BYTE_POS_PRESS_OUT	 		16
#define BYTE_POS_DR_MEASURE_STATE	20
#define BYTE_POS_DR_MEASURE_TIME	21


int tx_message_init(tx_message_t *self)
{
	memset(self, 0, sizeof(*self));
	memset(self->message, 0, TX_MESSAGE_SIZE);
	return 0;
}

int tx_message_deinit(tx_message_t *self)
{
	return 0;
}

void tx_message_increase_id(tx_message_t *self)
{
	uint32_t id;
	memcpy(&id, self->message + BYTE_POS_ID, sizeof(id)); // get current id
	id++; // increase id
	memcpy(self->message + BYTE_POS_ID, (char*)&id, sizeof(id)); // copy increased id
}

void tx_message_set_adc_dr_cnt(tx_message_t *self, int32_t val)
{
	memcpy(self->message + BYTE_POS_ADC_DR, (char*)&val, sizeof(val));
}

void tx_message_set_adc_dr_average_cnt(tx_message_t *self, int32_t val)
{
	memcpy(self->message + BYTE_POS_ADC_DR_AV, (char*)&val, sizeof(val));
}

void tx_message_set_hv_out_V(tx_message_t *self, int16_t val)
{
	memcpy(self->message + BYTE_POS_HV_OUT, (char*)&val, sizeof(val));
}

void tx_message_set_hv_polarity(tx_message_t *self, int8_t val)
{
	memcpy(self->message + BYTE_POS_HV_POL, (char*)&val, sizeof(val));
}

void tx_message_set_range(tx_message_t *self, int8_t val)
{
	memcpy(self->message + BYTE_POS_RANGE, (char*)&val, sizeof(val));
}

void tx_message_set_press_out_kPa(tx_message_t *self, int32_t val)
{
	memcpy(self->message + BYTE_POS_PRESS_OUT, (char*)&val, sizeof(val));
}

void tx_message_set_adc_dr_measure_state(tx_message_t *self, uint8_t val)
{
	memcpy(self->message + BYTE_POS_DR_MEASURE_STATE, (char*)&val, sizeof(val));
}

void tx_message_set_adc_dr_measure_time(tx_message_t *self, uint16_t val)
{
	memcpy(self->message + BYTE_POS_DR_MEASURE_TIME, (char*)&val, sizeof(val));
}


char* tx_message_get(tx_message_t *self)
{
	return self->message;
}

size_t tx_message_size(void)
{
	return TX_MESSAGE_SIZE;
}


