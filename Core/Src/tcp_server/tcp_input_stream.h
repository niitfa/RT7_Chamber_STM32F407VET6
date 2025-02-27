/*
 * tcp_input_stream.h
 *
 *  Created on: Aug 20, 2024
 *      Author: Kirill
 */

#ifndef SRC_TCP_SERVER_TCP_INPUT_STREAM_H_
#define SRC_TCP_SERVER_TCP_INPUT_STREAM_H_

#include "socket.h"

#define INPUT_MESSAGE_SIZE 8
#define INPUT_MESSAGE_BUFFER_SIZE INPUT_MESSAGE_SIZE

typedef struct
{
	uint8_t sn;
	uint16_t port;
	uint8_t flag;
	uint32_t w5500RebootCounterMax;
} tcp_input_stream_init_data_t;

typedef struct
{
	uint8_t stat;
	char msg		[INPUT_MESSAGE_SIZE];
	char msgTemp	[INPUT_MESSAGE_SIZE];
	char buff		[INPUT_MESSAGE_BUFFER_SIZE];
	int currentBuffIndex;
	int bytesReceived;
	uint16_t msgSize;
	tcp_input_stream_init_data_t initData;

	uint8_t handlerEnabled;
	uint8_t isConnected;

	uint32_t w5500RebootCounter;

	//tcp_input_state_t state;
} tcp_input_stream_t;

int 	tcp_input_stream_init(tcp_input_stream_t *self, tcp_input_stream_init_data_t tcpInit);
int 	tcp_input_stream_deinit(tcp_input_stream_t *self);
char* 	tcp_input_stream_get_message(tcp_input_stream_t *self);
int 	tcp_input_stream_message_size();
int 	tcp_input_stream_routine(tcp_input_stream_t *self);
void 	tcp_input_stream_enable_handler(tcp_input_stream_t *self);
void 	tcp_input_stream_disable_handler(tcp_input_stream_t *self);
int 	tcp_input_stream_close_socket(tcp_input_stream_t *self);
uint8_t tcp_input_stream_is_connected(tcp_input_stream_t *self);

#endif /* SRC_TCP_SERVER_TCP_INPUT_STREAM_H_ */
