/*
 * button.h
 *
 *  Created on: Sep 20, 2024
 *      Author: Kirill
 */

#ifndef SRC_KEYBOARD_BUTTON_H_
#define SRC_KEYBOARD_BUTTON_H_

#include "stm32f4xx.h"

typedef enum
{
	BUTTON_NOT_ACTIVE,
	BUTTON_FILTERING,
	BUTTON_SHORT_PRESS,
	BUTTON_SHORT_HOLD,
	BUTTON_LONG_PRESS,
	BUTTON_LONG_HOLD,
	BUTTON_RELEASE

} button_state_t;

typedef struct
{
	GPIO_TypeDef* 	port;
	uint16_t 		pin;
	GPIO_PinState	unpressedState;
	uint16_t		cyclesFiltering;
	uint16_t 		cyclesLongDelay;

} button_init_data_t;

typedef struct
{
	button_init_data_t initData;
	button_state_t state;
	uint16_t filteringCounter;
	uint16_t longDelayCounter;
	uint8_t currentButtonState;
	uint8_t previousButtonState;

	void (*short_press_cb) (void);
	void (*long_press_cb) (void);
	void (*release_cb) (void);

} button_t;

int button_init(button_t* self, button_init_data_t initData);
int button_deinit(button_t* self, button_init_data_t initData);

void button_reg_short_cb(button_t* self, void (*callback) (void));
void button_reg_long_cb(button_t* self, void (*callback) (void));
void button_reg_release_cb(button_t* self, void (*callback) (void));

void button_reset_cbs(button_t* self);
void button_routine(button_t* self);


#endif /* SRC_KEYBOARD_BUTTON_H_ */
