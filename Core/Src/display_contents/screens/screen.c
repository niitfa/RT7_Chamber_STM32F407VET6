/*
 * screen.c
 *
 *  Created on: Oct 15, 2024
 *      Author: Kirill
 */

#include "screen.h"

void screen_init(screen_t* self)
{
	self->vtable_->init(self);
}

void screen_draw(screen_t* self)
{
	self->vtable_->draw(self);
}

void screen_update(screen_t* self)
{
	self->vtable_->update(self);
}

void screen_register_keyboard(screen_t* self, keyboard_t* keyboard)
{
	self->vtable_->reg_keyboard_cbs(self, keyboard);
}


