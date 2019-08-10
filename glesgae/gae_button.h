#ifndef _GAE_BUTTON_H_
#define _GAE_BUTTON_H_

#include "gae_rect.h"
#include "gae_point.h"

enum gae_button_state
{	gae_button_inactive = 0
,	gae_button_active
,	gae_button_hover
,	gae_button_pressed
};

struct gae_button_s;
typedef void (*gae_button_callback_t)(void* data);

typedef struct gae_button_s {
	gae_sprite_sheet_t frames;
	void* userData;
	
	gae_button_callback_t onEnter;
	gae_button_callback_t onExit;
	gae_button_callback_t onPressed;
	gae_button_callback_t onReleased;
	
	gae_rect_t dims;
	
	enum gae_button_state state;
} gae_button_t;

gae_button_t* gae_button_init(gae_button_t* button, const char* buttonDef, gae_point_2d_t* const position, void* userData);
gae_button_t* gae_button_update(gae_button_t* button, gae_point_2d_t* const cursor, int isClicked);
gae_button_t* gae_button_destroy(gae_button_t* button);

#endif
