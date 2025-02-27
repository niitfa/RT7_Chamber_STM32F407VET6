/*
 * widget.h
 *
 *  Created on: Oct 16, 2024
 *      Author: Kirill
 */

#ifndef SRC_DISPLAY_CONTENTS_WIDGETS_WIDGET_H_
#define SRC_DISPLAY_CONTENTS_WIDGETS_WIDGET_H_

struct widget_data_t;

typedef struct
{
	struct widget_vtable_* vtable_;
	struct widget_data_t* data;
} widget_t;

struct widget_vtable_
{
	void (*draw)(widget_t* self, int form);
	void (*erase)(widget_t* self);
	void (*update)(widget_t* self);
};

void widget_draw(widget_t* self, int form);
void widget_erase(widget_t* self);
void widget_update(widget_t* self);

#endif /* SRC_DISPLAY_CONTENTS_WIDGETS_WIDGET_H_ */
