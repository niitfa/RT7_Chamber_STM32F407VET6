/*
 * button.c
 *
 *  Created on: Sep 20, 2024
 *      Author: Kirill
 */

#include "button.h"
#include <string.h>
//#include "button_callbacks.h"

static uint8_t button_is_pressed(button_t* self);
static void empty_cb_(void) {};

int button_init(button_t* self, button_init_data_t initData)
{
	memset(self, 0, sizeof(*self));
	self->initData 			= initData;
	self->short_press_cb 	= empty_cb_;
	self->long_press_cb 	= empty_cb_;
	self->release_cb		= empty_cb_;
	self->state 			= BUTTON_NOT_ACTIVE;
	self->filteringCounter	= 0;
	self->longDelayCounter	= 0;
	self->currentButtonState = self->initData.unpressedState;
	self->previousButtonState = self->initData.unpressedState;
	return 0;
}

int button_deinit(button_t* self, button_init_data_t initData)
{
	return 0;
}

void button_reg_short_cb(button_t* self, void (*callback) (void))
{
	self->short_press_cb = callback;
}

void button_reg_long_cb(button_t* self, void (*callback) (void))
{
	self->long_press_cb = callback;
}

void button_reg_release_cb(button_t* self, void (*callback) (void))
{
	self->release_cb = callback;
}

void button_reset_cbs(button_t* self)
{
	self->short_press_cb 	= empty_cb_;
	self->long_press_cb 	= empty_cb_;
	self->release_cb		= empty_cb_;
}

void button_routine(button_t* self)
{
	self->currentButtonState = button_is_pressed(self);
	switch(self->state)
	{
	case BUTTON_NOT_ACTIVE:
		self->filteringCounter = 0;
		self->longDelayCounter = 0;
		if(self->currentButtonState && !self->previousButtonState)
		{
			self->state = BUTTON_FILTERING;
		}
		break;
	case BUTTON_FILTERING:
		if(self->currentButtonState)
		{
			self->filteringCounter++;
		}
		else
		{
			self->state = BUTTON_NOT_ACTIVE;
		}
		if(self->filteringCounter >= self->initData.cyclesFiltering)
		{
			self->state = BUTTON_SHORT_PRESS;
		}
		break;
	case BUTTON_SHORT_PRESS:
		self->short_press_cb();
		self->state = BUTTON_SHORT_HOLD;
		break;
	case BUTTON_SHORT_HOLD:
		if(self->currentButtonState)
		{
			self->longDelayCounter++;
		}
		else
		{
			self->state = BUTTON_RELEASE;
		}

		if(self->longDelayCounter >= self->initData.cyclesLongDelay)
		{
			self->state = BUTTON_LONG_PRESS;
		}
		break;
	case BUTTON_LONG_PRESS:
		self->long_press_cb();
		self->state = BUTTON_LONG_HOLD;
		break;
	case BUTTON_LONG_HOLD:
		if(!self->currentButtonState)
		{
			self->state = BUTTON_RELEASE;
		}
		break;
	case BUTTON_RELEASE:
		self->release_cb();
		self->state = BUTTON_NOT_ACTIVE;
		break;
	}
	self->previousButtonState = self->currentButtonState;
}

static uint8_t button_is_pressed(button_t* self)
{
	return self->initData.unpressedState != HAL_GPIO_ReadPin(self->initData.port, self->initData.pin);
}
