/*
 * widget.c
 *
 *  Created on: Oct 16, 2024
 *      Author: Kirill
 */

#include "widget.h"

void widget_draw(widget_t* self, int form)
{
	self->vtable_->draw(self, form);
}
void widget_erase(widget_t* self)
{
	self->vtable_->erase(self);
}
void widget_update(widget_t* self)
{
	self->vtable_->update(self);
}


