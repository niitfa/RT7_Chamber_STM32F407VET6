/*
 * screen_1.c
 *
 *  Created on: Oct 15, 2024
 *      Author: Kirill
 */
#include "stm32f4xx.h"

#include "screen_1.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "ssd1306.h"
#include "ssd1306_fonts.h"

#include "general_task.h"

extern general_task_t task;

// Fonts, position constants
static const SSD1306_Font_t* TEXT_FONT = &Font_7x10;

static const int HEAD_X 	= 3;
static const int DESC_X 	= 3;
static const int VALUE_X 	= 30;
static const int UNIT_X 	= 90;

static const int HEAD_Y 	= 3;
static const int DR_Y 		= 26;
static const int HV_Y 		= 40;
static const int PRESS_Y 	= 54;

static const int DR_DESC_X = DESC_X;
static const int DR_DESC_Y = DR_Y;
static const int DR_VAL_X = VALUE_X;
static const int DR_VAL_Y = DR_Y;
static const int DR_UNIT_X = UNIT_X;
static const int DR_UNIT_Y = DR_Y;

static const int HV_DESC_X = DESC_X;
static const int HV_DESC_Y = HV_Y;
static const int HV_VAL_X = VALUE_X;
static const int HV_VAL_Y = HV_Y;
static const int HV_UNIT_X = UNIT_X;
static const int HV_UNIT_Y = HV_Y;

static const int PRESS_DESC_X = DESC_X;
static const int PRESS_DESC_Y = PRESS_Y;
static const int PRESS_VAL_X = VALUE_X;
static const int PRESS_VAL_Y = PRESS_Y;
static const int PRESS_UNIT_X = UNIT_X;
static const int PRESS_UNIT_Y = PRESS_Y;

static uint8_t initialized = 0;

struct screen_data_t
{
	int counter;

	// ADC values
	int64_t adcDRValueCurr, adcDRValuePrev;
	int64_t adcHVValueCurr, adcHVValuePrev;
	int64_t adcPRValueCurr, adcPRValuePrev;

	int64_t adcDRAverValueCurr, adcDRAverValuePrev;

	// ok button ready
	uint8_t isOkCharged;
};

static struct screen_data_t data;

static void init_(screen_t* self)
{
	memset(&data, 0, sizeof(data));

	self->data->adcDRValueCurr = 0;
	self->data->adcDRValuePrev = 0;
	self->data->adcHVValueCurr = 0;
	self->data->adcHVValuePrev = 0;
}

static void update_(screen_t* self)
{

	self->data->adcDRValueCurr = adc_get_cnt(&task.adcDoseRate);
	self->data->adcHVValueCurr = adc_get_cnt(&task.adcHV);
	self->data->adcPRValueCurr = adc_get_cnt(&task.adcPressure);

	self->data->adcDRAverValueCurr = adc_monitor_get_average_signal_value(&task.adcDRMonitor) * 1e+6;

	/*if(self->data->adcDRValuePrev != self->data->adcDRValueCurr)
	{
	}

	if(self->data->adcHVValuePrev != self->data->adcHVValueCurr)
	{
	}

	if(self->data->adcPRValuePrev != self->data->adcPRValueCurr)
	{
	}*/

	ssd1306_Fill(Black);

	// HEAD
	ssd1306_SetCursor(HEAD_X, HEAD_Y);
	ssd1306_WriteString("________________", *TEXT_FONT, White);

	// DR
	ssd1306_SetCursor(DR_DESC_X, DR_DESC_Y);
	ssd1306_WriteString("DR:", *TEXT_FONT, White);
	ssd1306_SetCursor(DR_VAL_X, DR_VAL_Y);
	//ssd1306_WriteInt((int)adc_get_vout(&task.adcDoseRate) * 1e+6, *TEXT_FONT, White);
	ssd1306_WriteInt(adc_get_cnt(&task.adcDoseRate), *TEXT_FONT, White);
	ssd1306_SetCursor(DR_UNIT_X, DR_UNIT_Y);
	ssd1306_WriteString("uV", *TEXT_FONT, White);

	// HV
	ssd1306_SetCursor(HV_DESC_X, HV_DESC_Y);
	ssd1306_WriteString("HV:", *TEXT_FONT, White);
	ssd1306_SetCursor(HV_VAL_X, HV_VAL_Y);
	//ssd1306_WriteInt((int)hv_get_output_voltage_V(&task.hv_system), *TEXT_FONT, White);
	ssd1306_WriteInt(adc_get_cnt(&task.adcHV), *TEXT_FONT, White);
	ssd1306_SetCursor(HV_UNIT_X, HV_UNIT_Y);
	ssd1306_WriteString("V", *TEXT_FONT, White);

	// Pressure
	ssd1306_SetCursor(PRESS_DESC_X, PRESS_DESC_Y);
	ssd1306_WriteString("PR:", *TEXT_FONT, White);
	ssd1306_SetCursor(PRESS_VAL_X, PRESS_VAL_Y);
	//ssd1306_WriteInt(adc_get_vout(&task.adcPressure) * task.pressureCoeff, *TEXT_FONT, White);
	ssd1306_WriteInt(adc_get_cnt(&task.adcPressure), *TEXT_FONT, White);
	ssd1306_SetCursor(PRESS_UNIT_X, PRESS_UNIT_Y);
	ssd1306_WriteString("Pa", *TEXT_FONT, White);

	ssd1306_UpdateScreen();




	// Average
	/*if(self->data->adcDRAverValuePrev != self->data->adcDRAverValueCurr)
	{
	} */


	self->data->adcDRValuePrev = self->data->adcDRValueCurr;
	self->data->adcHVValuePrev = self->data->adcHVValueCurr;
	self->data->adcPRValuePrev = self->data->adcPRValueCurr;

	self->data->adcDRAverValuePrev = self->data->adcDRAverValueCurr;
}

static void draw_(screen_t* self)
{
	ssd1306_Fill(Black);
	ssd1306_SetCursor(3, 3);
	ssd1306_WriteString("Loading...", *TEXT_FONT, White);
	ssd1306_UpdateScreen();
}

static void reg_keyboard_cbs_(screen_t* self, keyboard_t* keyboard)
{
	if(keyboard)
	{
		keyboard_reset_cbs(keyboard);
	}
}

static struct screen_vtable_ methods = {
	init_,
	draw_,
	update_,
	reg_keyboard_cbs_
};

static screen_t single_screen = {
	&methods,
	&data
};

screen_t* screen_1_instance()
{
	if(!initialized)
	{
		screen_init(&single_screen);
		initialized = 1;
	}
	return &single_screen;
}

