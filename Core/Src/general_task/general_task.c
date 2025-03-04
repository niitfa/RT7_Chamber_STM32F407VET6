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

#include "screen_1.h"

#include "adc_emulator.h"
#include "adc_AD7791.h"

#include "dac_emulator.h"
#include "dac_MCP4811_EP.h"

#include "range_select.h"

#include "ssd1306.h"
#include "ssd1306_fonts.h"

#include <time.h>
#include <stdlib.h>

/*
 * TO DO
 * 1) Take tcp routine state machine from stepper`s
 * 2) coeff for pressure adc (mV to Pa) - now 1000 in tx_message_set_pressure_Pa(...) in update()
 */


extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;
extern I2C_HandleTypeDef hi2c3;

//extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef* adctim;

#define TIMER_FREQUENCY (uint32_t)84000000

//extern TIM_HandleTypeDef htim7;

void general_task_init(general_task_t* self)
{
	HAL_Delay(100);
	memset(self, 0, sizeof(*self));

	self->loopPeriod_ms = 50;
	self->freqIT = TIMER_FREQUENCY / (adctim->Init.Period + 1) / (adctim->Init.Prescaler + 1);
	self->adcNoCnt = 0;

	self->pressureCoeff = 1e+6; // Pa per volt

	/* ADC */
	uint32_t adcWaitCycles = 20;
	uint8_t MR_word, BUF, UnB, BO, MD; // mode register options
	uint8_t FR_word, FS, CDIV; // filter register options
	double Vref_dose = 0;
	double Vref_hv = 0;
	double Vref_press = 0;

	/* ADC Dose Rate - bipolar */
	// Filter register mode
	FR_word = 0;
	FS = 0b011;
	CDIV = 0b00;

	FR_word = 0;
	FR_word += (FS << 0);
	FR_word += (CDIV << 4);

	// Mode register word
	BUF 	= 0b1;
	UnB 	= 0b0; // 0 - bipolar, 1 - unipolar
	BO 		= 0b0; // 0 - disable / 1 - enable burnout current
	MD 		= 0b00; // single - 0b10, cont - 0b00

	MR_word = 0;
	MR_word += (BUF << 1);
	MR_word += (UnB << 2);
	MR_word += (BO << 3);
	MR_word += (MD << 6);

	Vref_dose = 2.5;

	self->adcDoseRate = adc_AD7791_create(&hspi3, ADC_DOSE_SPI_CS_GPIO_Port, ADC_DOSE_SPI_CS_Pin, Vref_dose, FR_word, MR_word, adcWaitCycles);
	adc_init(&self->adcDoseRate);
	HAL_Delay(5);

	/* ADC HV - bipolar */
	// Filter register mode
	FR_word = 0;
	FS = 0b011;
	CDIV = 0b00;

	FR_word = 0;
	FR_word += (FS << 0);
	FR_word += (CDIV << 4);

	// Mode register word
	BUF 	= 0b1;
	UnB 	= 0b0; // 0 - bipolar, 1 - unipolar
	BO 		= 0b0; // 0 - disable / 1 - enable burnout current
	MD 		= 0b00; // single - 0b10, cont - 0b00

	MR_word = 0;
	MR_word += (BUF << 1);
	MR_word += (UnB << 2);
	MR_word += (BO << 3);
	MR_word += (MD << 6);

	Vref_hv = 2.5;

	self->adcHV = adc_AD7791_create(&hspi1, ADC_HV_SPI_CS_GPIO_Port, ADC_HV_SPI_CS_Pin, Vref_hv, FR_word, MR_word, adcWaitCycles + 10);
	adc_init(&self->adcHV);
	HAL_Delay(5);

	/* ADC Pressure - unipolar */
	// Filter register mode
	FR_word = 0;
	FS = 0b011;
	CDIV = 0b00;

	FR_word = 0;
	FR_word += (FS << 0);
	FR_word += (CDIV << 4);

	// Mode register word
	BUF 	= 0b1;
	UnB 	= 0b1; // 0 - bipolar, 1 - unipolar
	BO 		= 0b0; // 0 - disable / 1 - enable burnout current
	MD 		= 0b00; // single - 0b10, cont - 0b00

	MR_word = 0;
	MR_word += (BUF << 1);
	MR_word += (UnB << 2);
	MR_word += (BO << 3);
	MR_word += (MD << 6);

	Vref_press = 2.5;

	self->adcPressure = adc_AD7791_create(&hspi1, ADC_PRESS_SPI_CS_GPIO_Port, ADC_PRESS_SPI_CS_Pin, Vref_press, FR_word, MR_word, adcWaitCycles + 20);
	adc_init(&self->adcPressure);
	HAL_Delay(5);

	/* ADC Dose Rate monitor */
	adc_monitor_init(&self->adcDRMonitor, &self->adcDoseRate, USR_ADC_TIM_IRQn);

	/* ADC HV monitor */
	adc_monitor_init(&self->adcHVMonitor, &self->adcHV, USR_ADC_TIM_IRQn);

	/* ADC Pressure monitor */
	adc_monitor_init(&self->adcPRMonitor, &self->adcPressure, USR_ADC_TIM_IRQn);

	/* DAC HV Input */
	//self->dacInputHV = dac_emulator_create(); // emulator
	self->dacInputHV = dac_MCP4811EP_create(&hspi2,
			DAC_SPI_CS_GPIO_Port, DAC_SPI_CS_Pin,
			NULL, 0,
			NULL, 0
			); // real

	dac_init(&self->dacInputHV);

	/* HV System */
	hv_init(&self->hv_system,
			&self->dacInputHV,
			&self->adcHV,
			HV_INPUT_SELECT_GPIO_Port,
			HV_INPUT_SELECT_Pin,
			500. / 1024, // Vmax = 500, 922  = 1024 * 0.9 // 922
 			0.535 / 500, //1. / 233.645, // 43k/10M //0.002,
			500
			);

	hv_set_output_voltage_adc_offset(&self->hv_system, Vref_hv / 2);

	/* Select range pin */
	set_adc_dose_range_select_pin(SENSOR_RANGE_SELECT_GPIO_Port, SENSOR_RANGE_SELECT_Pin);
	select_broad_adc_dose_range();


	/* SD Card*/

	HAL_Delay(5);
	/* Ethernet */
	wiz_NetInfo gWIZNETINFO = {
			.mac 	= {0xed, 0xa2, 0xb3, 0xff, 0xfe, 0xfd},
			.ip 	= {169, 254, 206, 240}, // 169.254.206.240
			.sn 	= {255, 255, 255, 0},
			.gw		= {169, 254, 206, 240},
			.dns 	= {0, 0, 0, 0},
			.dhcp 	= NETINFO_STATIC
	};

	W5500_SetAddress(gWIZNETINFO);
	W5500_Reboot();

	/* TCP server sockets */
	tcp_output_stream_init_data_t tcpOutputInit;
	tcpOutputInit.sn = 0;
	tcpOutputInit.port = 11151;
	tcpOutputInit.flag = SF_IO_NONBLOCK;
	tcpOutputInit.hinput = &self->tcpInput;
	tcpOutputInit.closeSocketCounterMax = 50;

	tcp_output_stream_init(&self->tcpOutput, tcpOutputInit);

	tcp_input_stream_init_data_t tcpInputInit;
	tcpInputInit.sn = 1;
	tcpInputInit.port = 11152;
	tcpInputInit.flag = SF_IO_NONBLOCK;
	tcpInputInit.w5500RebootCounterMax = 50;
	tcp_input_stream_init(&self->tcpInput, tcpInputInit);

	self->cycleCounter = 0;
	self->cycleCounterMax = self->loopPeriod_ms;
}

void general_task_setup(general_task_t* self)
{
	//ILI9341_Init();

	// init display
	ssd1306_Init();
	general_task_switch_screen(self, screen_1_instance());
	tcp_input_stream_enable_handler(&self->tcpInput);
	/* HV ADC Start Calibration (offset measurement) */
	//adc_monitor_start_measurement(&self->adcHVMonitor, self->freqIT * 2 / 3);
}

void general_task_loop(general_task_t* self)
{
	keyboard_routine(&self->keyboard);
	if(!self->cycleCounter)
	{
		self->cycleCounter = self->cycleCounterMax;

		// Output message
		HAL_NVIC_DisableIRQ(USR_ADC_TIM_IRQn);
		tx_message_set_adc_dr_uV(&self->txMessage, (int32_t)(adc_get_vout(&self->adcDoseRate) * 1e+6));
		tx_message_set_adc_dr_average_uV(&self->txMessage, (int32_t)(adc_monitor_get_average_signal_value(&self->adcDRMonitor) * 1e+6));
		tx_message_set_hv_out_mV(&self->txMessage, (int32_t)(hv_get_output_voltage_V(&self->hv_system) * 1e+3)); // hv offset!!!
		tx_message_set_press_out_Pa(&self->txMessage, adc_get_vout(&self->adcPressure) * self->pressureCoeff);
		tx_message_set_adc_dr_measure_state(&self->txMessage, adc_monitor_get_measurement_state(&self->adcDRMonitor));
		tx_message_set_adc_dr_measure_time(&self->txMessage, adc_monitor_get_measurement_cycle_no(&self->adcDRMonitor) * 3 / self->freqIT );
		HAL_NVIC_EnableIRQ(USR_ADC_TIM_IRQn);

		tcp_output_stream_set_message(&self->tcpOutput, tx_message_get(&self->txMessage), tx_message_size());
		tcp_output_stream_routine(&self->tcpOutput);

		// Input message
		tcp_input_stream_routine(&self->tcpInput);

		// Update screen
		screen_update(self->currentScreen);

		// debug!!!
		//general_task_timer_interrupt(self);
	}

	self->cycleCounter--;
	HAL_Delay(1);
}

void general_task_timer_interrupt(general_task_t* self)
{
	switch(self->adcNoCnt)
	{
	case 0:
		tx_message_increase_id(&self->txMessage);
		/* Dose ADC*/
		adc_update(&self->adcDoseRate, NULL);
		adc_monitor_update(&self->adcDRMonitor);
		self->adcNoCnt++;
		break;
	case 1:
		/* HV ADC*/
		adc_update(&self->adcHV, NULL);
		adc_monitor_update(&self->adcHVMonitor);
		self->adcNoCnt++;
		break;
	case 2:
		/* Pressure ADC */
		adc_update(&self->adcPressure, NULL);
		adc_monitor_update(&self->adcPRMonitor);
		self->adcNoCnt = 0;
		break;
	}
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

