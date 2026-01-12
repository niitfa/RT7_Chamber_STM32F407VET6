/*
 * tcp_output_stream.c
 *
 *  Created on: Aug 9, 2024
 *      Author: Kirill
 */

#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include <tcp_output_stream.h>
#include "wiznet_api.h"

static int tcp_output_stream_open_socket(tcp_output_stream_t *self);
static int tcp_output_stream_listen_socket(tcp_output_stream_t *self);
static int tcp_output_stream_send(tcp_output_stream_t *self);
static int tcp_output_stream_reboot_socket(tcp_output_stream_t *self, int sent);

int tcp_output_stream_init(tcp_output_stream_t *self, tcp_output_stream_init_data_t tcpInit)
{
	memset(self, 0, sizeof(*self));
	self->stat = 0;
	self->initData = tcpInit;
	self->msgSize = 0;
	self->isConnected = 0;
	self->closeSocketCounter = self->initData.closeSocketCounterMax;
	memset(self->msg, 0, MAX_OUTPUT_MESSAGE_SIZE);
	return 0;
}

int tcp_output_stream_deinit(tcp_output_stream_t *self)
{
	return 0;
}

int tcp_output_stream_set_message(tcp_output_stream_t *self, char* message, uint16_t size)
{
	memset(self->msg, 0, MAX_OUTPUT_MESSAGE_SIZE);
	if(size < MAX_OUTPUT_MESSAGE_SIZE)
	{
		self->msgSize = size;
		memcpy(self->msg, message, size);
	}
	else
	{
		self->msgSize = MAX_OUTPUT_MESSAGE_SIZE;
		memcpy(self->msg, message, MAX_OUTPUT_MESSAGE_SIZE);
	}
	return 0;
}

int tcp_output_stream_routine(tcp_output_stream_t *self)
{
	// new state machine
	int sent = -1;
	self->isConnected = 0;
	switch(getSn_SR(self->initData.sn))
	{
	case SOCK_CLOSED:
		//int open = tcp_output_stream_open_socket(self);
		tcp_output_stream_open_socket(self);
		break;
	case SOCK_INIT:
		tcp_output_stream_listen_socket(self);
		break;
	case SOCK_ESTABLISHED:
		sent = tcp_output_stream_send(self);
		if(sent < 0)
		{
			disconnect(self->initData.sn);
		}
		else if (self == 0)
		{
			// закрыть сокет с дальнейшим переоткрытием, если несколько раз получили 0
			// для устранения этой фишки закомментить строку
			tcp_output_stream_reboot_socket(self, sent);
		}
		else
		{
			self->isConnected = 1;
		}
		break;

		// new
	case SOCK_CLOSE_WAIT:
		tcp_output_stream_close_socket(self);
		break;
	case SOCK_SYNSENT:
		tcp_output_stream_close_socket(self);
		break;
	}
	return 0;
}

uint8_t tcp_output_stream_is_connected(tcp_output_stream_t *self)
{
	return self->isConnected;
}

static int tcp_output_stream_open_socket(tcp_output_stream_t *self)
{
	self->stat = socket(self->initData.sn, Sn_MR_TCP, self->initData.port, self->initData.flag);
	return 0;
}

static int tcp_output_stream_listen_socket(tcp_output_stream_t *self)
{
	self->stat = listen(self->initData.sn);
	return 0;
}

static int tcp_output_stream_send(tcp_output_stream_t *self)
{
	return send(self->initData.sn, (uint8_t*)self->msg, self->msgSize);
}

int tcp_output_stream_close_socket(tcp_output_stream_t *self)
{
	disconnect(self->initData.sn);
	close(self->initData.sn);
	return 0;
}

static int tcp_output_stream_reboot_socket(tcp_output_stream_t *self, int sent)
{
	if (!sent) // sent == 0
	{
		if(self->closeSocketCounter == 0)
		{
			tcp_output_stream_close_socket(self);

			// reset delay counter after reboot
			self->closeSocketCounter = self->initData.closeSocketCounterMax;
		}
		else
		{
			// count delay
			self->closeSocketCounter--;
		}
	}
	else
	{
		// reset delay counter after reboot
		self->closeSocketCounter = self->initData.closeSocketCounterMax;
	}

	return 0;
}




