/*
 * general_task.c
 *
 *  Created on: Aug 30, 2024
 *      Author: Kirill
 */

#include "stm32f4xx_hal.h"
#include "main.h"
#include "general_task.h"
#include <string.h>
#include "uart_rx_handler.h"
#include "flash_data.h"

#include "screen_1.h"

#include "adc_emulator.h"
#include "adc_AD7791.h"
#include "adc_ADS1246.h"
#include "adc_ADS1242.h"

#include "dac_emulator.h"
#include "dac_MCP4811_EP.h"

#include "range_select.h"

#include "ssd1306.h"
#include "ssd1306_fonts.h"

#include <time.h>
#include <stdlib.h>

#define ADS1242_EN 0
#define ADS1246_EN 1
#define AD7791_EN 0

/*
 * TO DO
 * 1) Take tcp routine state machine from stepper`s
 * 2) coeff for pressure adc (mV to Pa) - now 1000 in tx_message_set_pressure_Pa(...) in update()
 */

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;
extern I2C_HandleTypeDef hi2c3;
extern UART_HandleTypeDef* conf_uart;

//extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef* adctim;

#define TIMER_FREQUENCY (uint32_t)84000000

//extern TIM_HandleTypeDef htim7;

void general_task_init(general_task_t* self)
{
	HAL_Delay(100);
	memset(self, 0, sizeof(*self));

	self->loopPeriod_ms = 10;
	self->freqIT = TIMER_FREQUENCY / (adctim->Init.Period + 1) / (adctim->Init.Prescaler + 1);
	self->adcNoCnt = 0;

	/* ADC */
	uint32_t adcWaitCycles = 120;
	/* ADC Dose Rate - bipolar */
// Filter register mode

	double Vref_dose = 2.5;
	uint8_t PGA_dose = 0b000;
	uint8_t DR_dose = 0b0010; // was 0b0010
	uint8_t SYS0_dose = DR_dose | (PGA_dose << 4);
	self->adcDoseRate = adc_ADS1246_create(&hspi3,
			ADC_DOSE_SPI_CS_GPIO_Port, ADC_DOSE_SPI_CS_Pin,
			ADC_DOSE_xDRDY_GPIO_Port, ADC_DOSE_xDRDY_Pin,
			//ADC_DOSE_START_GPIO_Port, ADC_DOSE_START_Pin,
			//ADC_DOSE_XPWDN_GPIO_Port, ADC_DOSE_XPWDN_Pin,
			NULL, 0,
			NULL, 0,
			Vref_dose,
			SYS0_dose,
			adcWaitCycles
			);


	adc_init(&self->adcDoseRate);
	HAL_Delay(5);


	/* ADC HV - bipolar */
	double Vref_hv = 2.5;
	uint8_t PGA_hv = 0b000;
	uint8_t DR_hv = 0b0010; // was 0b0010
	uint8_t SYS0_hv = DR_hv | (PGA_hv << 4);
	self->adcHV = adc_ADS1246_create(&hspi1,
			ADC_HV_SPI_CS_GPIO_Port, ADC_HV_SPI_CS_Pin,
			ADC_HV_xDRDY_GPIO_Port, ADC_HV_xDRDY_Pin,
			NULL, 0,
			NULL, 0,
			Vref_hv,
			SYS0_hv,
			adcWaitCycles
			);

	adc_init(&self->adcHV);
	HAL_Delay(5);

	/* ADC Pressure - unipolar */
	double Vref_press = 2.5;
	uint8_t PGA_press = 0b000;
	uint8_t DR_press = 0b0010; // was 0b0010
	uint8_t SYS0_press = DR_press | (PGA_press << 4);
	self->adcPressure= adc_ADS1246_create(&hspi1,
			ADC_PRESS_SPI_CS_GPIO_Port, ADC_PRESS_SPI_CS_Pin,
			ADC_PRESS_xDRDY_GPIO_Port, ADC_PRESS_xDRDY_Pin,
			NULL, 0,
			NULL, 0,
			Vref_press,
			SYS0_press,
			adcWaitCycles
			);

	adc_init(&self->adcPressure);

	/* ADC Dose Rate monitor */
	adc_monitor_init(&self->adcDRMonitor, &self->adcDoseRate, USR_ADC_TIM_IRQn);

	/* ADC HV monitor */
	adc_monitor_init(&self->adcHVMonitor, &self->adcHV, USR_ADC_TIM_IRQn);

	/* ADC Pressure monitor */
	adc_monitor_init(&self->adcPRMonitor, &self->adcPressure, USR_ADC_TIM_IRQn);

	/* Pressure sensor */
	int pressureOffsetkPa = 100; // kPa
	float kPaPerV = 20. * 1e+2 / 2.5;

	pressure_sensor_init(&self->pressureSensor, pressureOffsetkPa, kPaPerV, &self->adcPressure);

	/* DAC HV Input */
	mcp4822_init(&self->dacInputHV, &hspi2, DAC_SPI_CS_GPIO_Port,DAC_SPI_CS_Pin, NULL, 0);



	/* HV System */
	hv_init(&self->hv_system,
			&self->dacInputHV,
			&self->adcHV,
			HV_INPUT_SELECT_GPIO_Port,
			HV_INPUT_SELECT_Pin,
			500. / 4096, // Vmax = 500, 922  = 1024 * 0.9 // 922
 			0.535 / 500, //1. / 233.645, // 43k/10M //0.002,
			500
			);

	hv_set_output_voltage_adc_offset(&self->hv_system, Vref_hv / 2);

	/* Select range pin */
	set_adc_dose_range_select_pin(SENSOR_RANGE_SELECT_GPIO_Port, SENSOR_RANGE_SELECT_Pin);
	select_broad_adc_dose_range();


	HAL_Delay(5);
	/* Ethernet */	// default values
	uint8_t defIP[4] = {169, 254, 206, 12};
	uint16_t defInputPort = 22252;
	uint16_t defOutputPort = 22251;


	// actual values
	memcpy(self->ip, defIP, 4);
	self->inputPort = defInputPort;
	self->outputPort = defOutputPort;

	// reading from flash
	flash_data_t fdata = flash_data_read();
	if(fdata.input_port != 0xFFFF && fdata.input_port != 0x0000)
	{
		self->inputPort = fdata.input_port;
	}
	if(fdata.output_port != 0xFFFF && fdata.output_port != 0x0000)
	{
		self->outputPort = fdata.output_port;
	}
	if(*(uint32_t*)fdata.ip != 0xFFFFFFFF && *(uint32_t*)fdata.ip != 0x00000000)
	{
		memcpy(self->ip, fdata.ip, 4);
	}


	wiz_NetInfo gWIZNETINFO = {
			.mac 	= {0xed, 0xa2, 0xb3, 0xff, 0xfe, 0xa9},
			.ip 	= {self->ip[0], self->ip[1], self->ip[2], self->ip[3]}, // 169.254.206.240
			.sn 	= {255, 255, 255, 0},
			.gw		= {self->ip[0], self->ip[1], self->ip[2], 1}, // {169, 254, 206, 1},
			.dns 	= {0, 0, 0, 0},
			.dhcp 	= NETINFO_STATIC
	};

	/*wiz_NetInfo gWIZNETINFO = {
			.mac 	= {0xed, 0xa2, 0xb3, 0xff, 0xfe, 0xa9},
			.ip 	= {169, 254, 206, 240}, // 169.254.206.240
			.sn 	= {255, 255, 255, 0},
			.gw		= {169, 254, 206, 240}, // {169, 254, 206, 1},
			.dns 	= {0, 0, 0, 0},
			.dhcp 	= NETINFO_STATIC
	}; */

	W5500_SetAddress(gWIZNETINFO);
	W5500_Reboot();

	/* TCP server sockets */
	tcp_output_stream_init_data_t tcpOutputInit;
	tcpOutputInit.sn = 0;
	tcpOutputInit.port = self->outputPort;
	//tcpOutputInit.port = 11151;
	tcpOutputInit.flag = SF_IO_NONBLOCK;
	tcpOutputInit.hinput = &self->tcpInput;
	tcpOutputInit.closeSocketCounterMax = 50;

	tcp_output_stream_init(&self->tcpOutput, tcpOutputInit);

	tcp_input_stream_init_data_t tcpInputInit;
	tcpInputInit.sn = 1;
	tcpInputInit.port = self->inputPort;
	//tcpOutputInit.port = 11152;
	tcpInputInit.flag = SF_IO_NONBLOCK;
	tcpInputInit.w5500RebootCounterMax = 50;
	tcp_input_stream_init(&self->tcpInput, tcpInputInit);

	self->cycleCounter = 0;
	self->cycleCounterMax = self->loopPeriod_ms;
}

void general_task_setup(general_task_t* self)
{
	//ssd1306_Init();

	general_task_switch_screen(self, screen_1_instance());
	//screen_draw(self->currentScreen);

	tcp_input_stream_enable_handler(&self->tcpInput);
	memset(self->uart_buff, 0, UART_BUFF_SIZE);
	HAL_UART_Receive_IT(conf_uart, self->uart_buff, UART_BUFF_SIZE);
	HAL_Delay(1000);
}

void general_task_loop(general_task_t* self)
{
	//keyboard_routine(&self->keyboard);
	if(!self->cycleCounter)
	{
		self->cycleCounter = self->cycleCounterMax;

		// Output message
		HAL_NVIC_DisableIRQ(USR_ADC_TIM_IRQn);
		tx_message_set_adc_dr_cnt(&self->txMessage, (int32_t)(adc_get_cnt(&self->adcDoseRate))); // DONT uV, raw adc counts!!!!!
		tx_message_set_adc_dr_average_cnt(&self->txMessage, adc_monitor_get_average_value(&self->adcDRMonitor)); // DONT uV, raw adc counts!!!!!
		tx_message_set_hv_out_V(&self->txMessage, (int32_t)(hv_get_output_voltage_V(&self->hv_system))); // DONT mV, V!!!!
		tx_message_set_hv_polarity(&self->txMessage, hv_get_source_polarity(&self->hv_system)); // 1
		tx_message_set_range(&self->txMessage, get_current_adc_dose_range()); // 1
		tx_message_set_press_out_kPa(&self->txMessage, pressure_sensor_get_kPa(&self->pressureSensor));
		tx_message_set_adc_dr_measure_state(&self->txMessage, adc_monitor_get_measurement_state(&self->adcDRMonitor));
		tx_message_set_adc_dr_measure_time(&self->txMessage, adc_monitor_get_measurement_cycle_no(&self->adcDRMonitor)); // cycle, not time!!!!
		HAL_NVIC_EnableIRQ(USR_ADC_TIM_IRQn);

		tcp_output_stream_set_message(&self->tcpOutput, tx_message_get(&self->txMessage), tx_message_size());
		tcp_output_stream_routine(&self->tcpOutput);
		tcp_input_stream_routine(&self->tcpInput);

		// Update screen
		//screen_update(self->currentScreen);
	}

	self->cycleCounter--;
	HAL_Delay(1);
}

void general_task_timer_interrupt(general_task_t* self)
{
	int nextStateCode = 0;
	switch(self->adcNoCnt)
	{
	case 0:
		/* Dose ADC*/
		//adc_update(&self->adcDoseRate, NULL);
		//adc_monitor_update(&self->adcDRMonitor);
		//self->adcNoCnt++;

		adc_update(&self->adcDoseRate, (void*)&nextStateCode);
		if(nextStateCode == ADS1246_MEASURE) // current state is ADS1246_CHECK_xDRDY
		{
			tx_message_increase_id(&self->txMessage);
			adc_monitor_update(&self->adcDRMonitor);
		}
		else
		{
			self->adcNoCnt++;
		}
		break;
	case 1:
		/* HV ADC*/
		//adc_update(&self->adcHV, NULL);
		//adc_monitor_update(&self->adcHVMonitor);
		//self->adcNoCnt++;

		adc_update(&self->adcHV, (void*)&nextStateCode);
		if(nextStateCode == ADS1246_MEASURE) // current state is ADS1246_CHECK_xDRDY
		{
			adc_monitor_update(&self->adcHVMonitor);
		}
		else
		{
			self->adcNoCnt++;
		}

		break;
	case 2:
		/* Pressure ADC */
		//adc_update(&self->adcPressure, NULL);
		//adc_monitor_update(&self->adcPRMonitor);
		//self->adcNoCnt = 0;

		adc_update(&self->adcPressure, (void*)&nextStateCode);
		if(nextStateCode == ADS1246_MEASURE) // current state is ADS1246_CHECK_xDRDY
		{
			adc_monitor_update(&self->adcPRMonitor);
		}
		else
		{
			self->adcNoCnt = 0;
		}

		break;
	}
}

void general_task_uart_recv_callback(general_task_t* self)
{
	uart_handle_rx_message(conf_uart, self->uart_buff);
	memset(self->uart_buff, 0, UART_BUFF_SIZE);
	HAL_UART_Receive_IT(conf_uart, self->uart_buff, UART_BUFF_SIZE);
}


void general_task_switch_screen(general_task_t* self, screen_t* screen)
{
	//screen_register_keyboard(screen, &self->keyboard);
	screen_draw(screen);
	self->currentScreen = screen;
}

uint16_t general_task_get_loop_period_ms(general_task_t* self)
{
	return self->loopPeriod_ms;
}

