/*
 * uart_rx_handler.c
 *
 *  Created on: Apr 7, 2025
 *      Author: Kirill
 */
#include "uart_rx_handler.h"
#include "flash_data.h"
//#include "general_task.h"
#include <string.h>

//extern general_task_t task;

typedef enum
{
	COMM_EMPTY = 0x00,
	COMM_SET_CONF = 0x01,
	COMM_GET_CONF = 0x02,
	COMM_PING = 0x03
} command_code_t;

typedef enum
{
	RESP_OK = 0x01,
	RESP_ERR = 0x02
} response_code_t;

void uart_handle_rx_message(UART_HandleTypeDef* huart, uint8_t *msg)
{
	// 0 byte - command
	// 1 - 8 bytes - parameters
	// msg:
	// 0 - ip[0]
	// 1 - ip[1]
	// 2 - ip[2]
	// 3 - ip[3]
	// 4-5 - output port
	// 6-7 - input port
	const int TIMEOUT = 50;

	uint8_t code_ok = RESP_OK;
	flash_data_t data;
	//uint8_t code_err = RESP_ERR;
	command_code_t command_code = *(int*)msg;

	const int TX_BUFF_SIZE = 8;
	uint8_t buff[TX_BUFF_SIZE];
	memset(buff, 0, TX_BUFF_SIZE);

	switch(command_code)
	{
	case COMM_EMPTY:
		break;
	case COMM_SET_CONF:
		memcpy(data.ip, msg + 1, 4);
		memcpy(&data.input_port, msg + 7, 2);
		memcpy(&data.output_port, msg + 5, 2);
		flash_data_write(data);
		HAL_UART_Transmit(huart, &code_ok, 1, TIMEOUT);//
		break;
	case COMM_GET_CONF:
		data = flash_data_read();
		memcpy(buff + 0, data.ip, 4);
		memcpy(buff + 6, &data.input_port, 2);
		memcpy(buff + 4, &data.output_port, 2);
		HAL_UART_Transmit(huart, buff, TX_BUFF_SIZE, TIMEOUT);
		break;
	case COMM_PING:
		HAL_UART_Transmit(huart, &code_ok, 1, TIMEOUT);
		break;
	}

}


