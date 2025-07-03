/*
 * tx_message.h
 *
 *  Created on: Aug 16, 2024
 *      Author: Kirill
 */

#ifndef SRC_TX_MESSAGE_TX_MESSAGE_H_
#define SRC_TX_MESSAGE_TX_MESSAGE_H_

#include <stdint.h>
#include <stddef.h>

#define TX_MESSAGE_SIZE 23

typedef struct
{
	char message[TX_MESSAGE_SIZE];

} tx_message_t;

int tx_message_init(tx_message_t *self);
int tx_message_deinit(tx_message_t *self);

void tx_message_increase_id(tx_message_t *self); // 4
void tx_message_set_adc_dr_cnt(tx_message_t *self, int32_t val); // 4
void tx_message_set_adc_dr_average_cnt(tx_message_t *self, int32_t val); // 4

void tx_message_set_hv_out_V(tx_message_t *self, int16_t val); // 2
void tx_message_set_hv_polarity(tx_message_t *self, int8_t val); // 1
void tx_message_set_range(tx_message_t *self, int8_t val); // 1

void tx_message_set_press_out_kPa(tx_message_t *self, int32_t val); // 4
void tx_message_set_adc_dr_measure_state(tx_message_t *self, uint8_t val);  // 1
void tx_message_set_adc_dr_measure_time(tx_message_t *self, uint16_t val); // 2

char* tx_message_get(tx_message_t *self);
size_t tx_message_size(void);

#endif /* SRC_TX_MESSAGE_TX_MESSAGE_H_ */
