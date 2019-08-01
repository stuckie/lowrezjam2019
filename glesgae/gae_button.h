#ifndef _GAE_BUTTON_H_
#define _GAE_BUTTON_H_

#include "gae.h"

enum gae_button_state
{	gae_button_inactive = 0
,	gae_button_active
,	gae_button_hover
,	gae_button_pressed
};

typedef void (*gae_button_callback_t)(struct gae_button_s* button, void* data);

typedef struct gae_button_s {
	gae_array_t frames;
	gae_frame_t* frame;
	void* userData;
	
	gae_button_callback_t onEnter;
	gae_button_callback_t onExit;
	gae_button_callback_t onPressed;
	gae_button_callback_t onReleased;
	
	gae_rect_t pos;
	
	enum gae_button_state state;
} gae_button_t;

void gae_button_default_onEnter(gae_button_t* button, void* data);
void gae_button_default_onExit(gae_button_t* button, void* data);
void gae_button_default_onPressed(gae_button_t* button, void* data);
void gae_button_default_onReleased(gae_button_t* button, void* data);

gae_button_t* gae_button_init(gae_button_t* button);
gae_button_t* gae_button_update(gae_button_t* button, gae_rect_t* const cursor);
gae_button_t* gae_button_destroy(gae_button_t* button);

#endif
