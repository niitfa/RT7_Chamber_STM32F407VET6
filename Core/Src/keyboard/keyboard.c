/*
 * keyboard.c
 *
 *  Created on: Sep 20, 2024
 *      Author: Kirill
 */

#include "keyboard.h"
#include <string.h>

void keyboard_routine(keyboard_t* self)
{
	button_routine(&self->up);
	button_routine(&self->down);
	button_routine(&self->ok);
	button_routine(&self->axisReset);
	button_routine(&self->step);
	button_routine(&self->motion);
}

void keyboard_reset_cbs(keyboard_t* self)
{

	button_reset_cbs(&self->up);
	button_reset_cbs(&self->down);
	button_reset_cbs(&self->ok);
	button_reset_cbs(&self->axisReset);
	button_reset_cbs(&self->step);
	button_reset_cbs(&self->motion);
}

