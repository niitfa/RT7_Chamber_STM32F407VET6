/*
 * tcp_input_stream.c
 *
 *  Created on: Aug 20, 2024
 *      Author: Kirill
 */

#include <rx_message.h>
#include "tcp_input_stream.h"
#include <string.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"

#include "wiznet_api.h"

//#define ENABLE_REBOOT 0

static int tcp_input_stream_open_socket(tcp_input_stream_t *self);
static int tcp_input_stream_listen_socket(tcp_input_stream_t *self);
static int tcp_input_stream_receive(tcp_input_stream_t *self);
static int tcp_input_stream_reboot_ethernet(tcp_input_stream_t *self, int sockOpen);

int tcp_input_stream_init(tcp_input_stream_t *self, tcp_input_stream_init_data_t tcpInit)
{
	memset(self, 0, sizeof(*self));
	memset(self->msg, 0, INPUT_MESSAGE_SIZE);
	memset(self->msgTemp, 0, INPUT_MESSAGE_SIZE);
	memset(self->buff, 0, INPUT_MESSAGE_BUFFER_SIZE);
	self->stat = 0;
	self->initData = tcpInit;
	self->currentBuffIndex = 0;
	self->bytesReceived = 0;
	self->msgSize = 0;
	self->handlerEnabled = 0;
	self->isConnected = 0;
	self->w5500RebootCounter = self->initData.w5500RebootCounterMax;

	//self->state = INPUT_SOCK_INIT;
	return 0;
}

int tcp_input_stream_deinit(tcp_input_stream_t *self)
{
	return 0;
}

char* tcp_input_stream_get_message(tcp_input_stream_t *self)
{
	return self->msg;
}

int tcp_input_stream_message_size()
{
	return INPUT_MESSAGE_SIZE; //size
}

int tcp_input_stream_routine(tcp_input_stream_t *self)
{
	// new state mashine
	int received = -1;
	self->isConnected = 0;
	switch(getSn_SR(self->initData.sn))
	{
	case SOCK_CLOSED:
		int open = tcp_input_stream_open_socket(self);
		tcp_input_stream_reboot_ethernet(self, open);
		break;
	case SOCK_INIT:
		tcp_input_stream_listen_socket(self);
		break;
	case SOCK_ESTABLISHED:
		received = tcp_input_stream_receive(self);
		if(received < 0)
		{
			tcp_input_stream_close_socket(self);
		}
		else
		{
			self->isConnected = 1;
		}
		break;
	}
	return 0;
}


void tcp_input_stream_enable_handler(tcp_input_stream_t *self)
{
	self->handlerEnabled = 1;
}
void tcp_input_stream_disable_handler(tcp_input_stream_t *self)
{
	self->handlerEnabled = 0;
}

uint8_t tcp_input_stream_is_connected(tcp_input_stream_t *self)
{
	return self->isConnected;
}

static int tcp_input_stream_open_socket(tcp_input_stream_t *self)
{
	self->stat = socket(self->initData.sn, Sn_MR_TCP, self->initData.port, self->initData.flag);
	return 0;
}

static int tcp_input_stream_listen_socket(tcp_input_stream_t *self)
{
	self->stat = listen(self->initData.sn);
	return 0;
}

static int tcp_input_stream_receive(tcp_input_stream_t *self)
{
	while (self->bytesReceived != INPUT_MESSAGE_SIZE)
	{
		int recv_size = recv(self->initData.sn, (uint8_t*)self->buff, INPUT_MESSAGE_BUFFER_SIZE);
		if (recv_size == 0)
		{
			return 0;
		}
		if (recv_size < 0)
		{
			return recv_size;
		}

		self->bytesReceived += recv_size;

		int i;
		for(i = 0; i < recv_size; ++i)
		{
			self->msgTemp[self->currentBuffIndex] = self->buff[i];
			self->currentBuffIndex++;
		}

	}

	memcpy(self->msg, self->msgTemp, INPUT_MESSAGE_SIZE);

	if(self->handlerEnabled)
	{
		rx_message_run_command(self->msg); // reaction
	}

	int tempBytesReceived = self->bytesReceived;
	memset(self->msgTemp, 0, INPUT_MESSAGE_SIZE);
	self->bytesReceived = 0;
	self->currentBuffIndex = 0;

	return tempBytesReceived;
}

int tcp_input_stream_close_socket(tcp_input_stream_t *self)
{
	disconnect(self->initData.sn);
	close(self->initData.sn);
	return 0;
}

static int tcp_input_stream_reboot_ethernet(tcp_input_stream_t *self, int sockOpen)
{
	if(sockOpen != SOCK_OK)
	{
		if(self->w5500RebootCounter == 0)
		{
			W5500_Reboot();
			// reset delay counter after reboot
			self->w5500RebootCounter = self->initData.w5500RebootCounterMax;
		}
		else
		{
			// count delay
			self->w5500RebootCounter--;
		}
	}
	else
	{
		// reset delay counter after ok open socket
		self->w5500RebootCounter = self->initData.w5500RebootCounterMax;
	}
	return 0;
}


