/*
 * general_task.h
 *
 *  Created on: Aug 30, 2024
 *      Author: Kirill
 */

#ifndef SRC_GENERAL_TASK_GENERAL_TASK_H_
#define SRC_GENERAL_TASK_GENERAL_TASK_H_

#include "keyboard.h"
#include "screen.h"
#include "tcp_output_stream.h"
#include "tcp_input_stream.h"
#include "wiznet_api.h"
#include "tx_message.h"
#include "adc.h"
#include "dac.h"
#include "adc_monitor.h"
#include "counter.h"
#include "high_voltage_system.h"
#include "user_sd.h"

typedef struct
{
	uint16_t loopPeriod_ms;
	uint16_t freqIT;

	/* Ethernet */
	tcp_output_stream_t tcpOutput;
	tcp_input_stream_t tcpInput;
	tx_message_t txMessage;

	/* Console */
	keyboard_t keyboard;
	screen_t* currentScreen;

	/* ACD */
	int adcNoCnt;
	adc_t adcDoseRate;
	adc_t adcHV;
	adc_t adcPressure;

	float pressureCoeff;

	adc_monitor_t adcDRMonitor;
	adc_monitor_t adcHVMonitor;
	adc_monitor_t adcPRMonitor;
	/* DAC */
	dac_t dacInputHV;

	/* HV System */
	high_voltage_system_t hv_system;

	/* ADC emulation counters */
	counter_t cntDoseRate;
	counter_t cntHV;

	/* SD */
	user_sd_t sd;
	int sd_mount, sd_open, sd_puts, sd_close;

	int cycleCounter, cycleCounterMax;
} general_task_t;

void general_task_init(general_task_t* self);
void general_task_setup(general_task_t* self);
void general_task_loop(general_task_t* self);
void general_task_timer_interrupt(general_task_t* self);

// other
void general_task_switch_screen(general_task_t* self, screen_t* screen);
uint16_t general_task_get_loop_period_ms(general_task_t* self);

#endif /* SRC_GENERAL_TASK_GENERAL_TASK_H_ */
