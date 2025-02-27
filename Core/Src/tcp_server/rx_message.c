/*
 * rx_message_handler.c
 *
 *  Created on: Sep 25, 2024
 *      Author: Kirill
 */

#include "rx_message.h"
#include "general_task.h"
#include <string.h>
#include "screen_1.h"
#include "range_select.h"

#include <stdlib.h>

extern general_task_t task;

void rx_message_run_command(char* msg)
{
	int32_t code = 0, param = 0;
	memcpy(&code, msg, sizeof(code));
	memcpy(&param, msg + 4, sizeof(param));

	switch(code)
	{
	case COMM_EMPTY:
		break;
	case COMM_SET_HV:
		// [param] = HV in Volts
		hv_set_abs_output_voltage_V(&task.hv_system, (double)abs(param));
		break;
	case COMM_START_MEAS:
		if(param >= 0) // [param] = seconds
		{
			adc_monitor_start_measurement(&task.adcDRMonitor, task.freqIT / 3 * param);
		}
		break;
	case COMM_RESET_MEAS:
		adc_monitor_reset_measurement(&task.adcDRMonitor);
		break;
	case COMM_SET_MEAS_RANGE:
		if(!param) // [param] = 0 - broad range, 1 - narrow range
		{
			select_broad_adc_dose_range();
		}
		else
		{
			select_narrow_adc_dose_range();
		}
		break;
	case COMM_SWITCH_HV:
		if(!param) // [param] = 0 - positive HV source, 1 - negative HV source
		{
			hv_select_positive_source(&task.hv_system);
		}
		else
		{
			hv_select_negative_source(&task.hv_system);
		}
		// gpio toggle
		break;
	}
}

int rx_message_keep_alive(char* msg)
{
	int32_t code = 0;
	memcpy(&code, msg, sizeof(code));

	if(code == COMM_KEEP_ALIVE)
	{
		return 1;
	}
	return 0;
}


