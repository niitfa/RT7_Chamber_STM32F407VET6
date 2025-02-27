/*
 * screen.h
 *
 *  Created on: Oct 15, 2024
 *      Author: Kirill
 */

#ifndef SRC_DISPLAY_CONTENTS_SCREEN_H_
#define SRC_DISPLAY_CONTENTS_SCREEN_H_

#include "keyboard.h"

struct screen_data_t;

typedef struct
{
	struct screen_vtable_* vtable_;
	struct screen_data_t* data;
} screen_t;

struct screen_vtable_
{
	void (*init)(screen_t*);
	void (*draw)(screen_t*);
	void (*update)(screen_t*);
	void (*reg_keyboard_cbs)(screen_t*, keyboard_t*);
};

void screen_init(screen_t* self);
void screen_draw(screen_t* self);
void screen_update(screen_t* self);
void screen_register_keyboard(screen_t* self, keyboard_t* keyboard);


#endif /* SRC_DISPLAY_CONTENTS_SCREEN_H_ */
