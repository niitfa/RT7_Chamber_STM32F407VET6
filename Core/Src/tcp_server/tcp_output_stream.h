/*
 * tcp_output_stream.h
 *
 *  Created on: Aug 9, 2024
 *      Author: Kirill
 */

#ifndef SRC_TCP_SERVER_TCP_OUTPUT_STREAM_H_
#define SRC_TCP_SERVER_TCP_OUTPUT_STREAM_H_

#include "socket.h"
#include "tcp_input_stream.h"

#define MAX_OUTPUT_MESSAGE_SIZE 128

typedef struct
{
	uint8_t sn;
	uint16_t port;
	uint8_t flag;
	tcp_input_stream_t* hinput;
	uint32_t closeSocketCounterMax;
} tcp_output_stream_init_data_t;

typedef struct
{
	uint8_t stat;
	char msg[MAX_OUTPUT_MESSAGE_SIZE];
	uint16_t msgSize;
	tcp_output_stream_init_data_t initData;
	uint8_t isConnected;

	uint32_t closeSocketCounter;
} tcp_output_stream_t;

int tcp_output_stream_init(tcp_output_stream_t *self, tcp_output_stream_init_data_t tcpInit);
int tcp_output_stream_deinit(tcp_output_stream_t *self);
int tcp_output_stream_set_message(tcp_output_stream_t *self, char* message, uint16_t size);
int tcp_output_stream_routine(tcp_output_stream_t *self);
int tcp_output_stream_close_socket(tcp_output_stream_t *self);
uint8_t tcp_output_stream_is_connected(tcp_output_stream_t *self);

#endif /* SRC_TCP_SERVER_TCP_OUTPUT_STREAM_H_ */
