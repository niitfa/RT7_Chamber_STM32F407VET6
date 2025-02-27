/*
 * rx_message_handler.h
 *
 *  Created on: Sep 25, 2024
 *      Author: Kirill
 */

#ifndef SRC_TCP_SERVER_RX_MESSAGE_HANDLER_H_
#define SRC_TCP_SERVER_RX_MESSAGE_HANDLER_H_

typedef enum
{
	COMM_EMPTY				= 0x00,
	COMM_SET_HV				= 0x01,
	COMM_START_MEAS			= 0x02,
	COMM_RESET_MEAS			= 0x03,
	COMM_SET_MEAS_RANGE 	= 0x04,
	COMM_SWITCH_HV		 	= 0x05,
	COMM_KEEP_ALIVE			= 0x20
} command_code_t;

void rx_message_run_command(char* msg);
int rx_message_keep_alive(char* msg);

#endif /* SRC_TCP_SERVER_RX_MESSAGE_HANDLER_H_ */
