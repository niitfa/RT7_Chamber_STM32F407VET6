/*
 * keyboard.h
 *
 *  Created on: Sep 20, 2024
 *      Author: Kirill
 */

#ifndef SRC_KEYBOARD_KEYBOARD_H_
#define SRC_KEYBOARD_KEYBOARD_H_

#include "stm32f4xx_hal.h"
#include "button.h"

typedef struct
{
	button_t
		up,
		down,
		ok,
		axisReset,
		step,
		motion;

} keyboard_t;

void keyboard_routine(keyboard_t* self);
void keyboard_reset_cbs(keyboard_t* self);



#endif /* SRC_KEYBOARD_KEYBOARD_H_ */
