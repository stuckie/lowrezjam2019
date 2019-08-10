#include "fishy_cast.h"

#include "gae.h"

typedef struct fishy_cast_s {
	gae_camera_t camera;
	gae_timer_t timer;
	
	int backSwing;
	int foreSwing;
	
	gae_sprite_sheet_t sprites;
	gae_graphics_texture_t ui;
	gae_rect_t uiRect;
} fishy_cast_t;

static int onStart(void* userData)
{
	fishy_cast_t* data = userData;
	
	gae_graphics_texture_init(&data->ui);
	gae_graphics_context_texture_load_from_file(gae_system.graphics.context, "data/lake_ui.bmp", &data->ui);
	
	data->uiRect.x = 0;
	data->uiRect.y = 48;
	data->uiRect.w = 64;
	data->uiRect.h = 16;
	
	return 0;
}

static int onUpdate(void* userData)
{
	(void)(userData);
	
	return 0;
}

static int onStop(void* userData)
{
	(void)(userData);
	
	return 0;
}

gae_state_t* fishy_cast_init(gae_state_t* state)
{
	fishy_cast_t* data = gae_alloc(sizeof(fishy_cast_t));
	
	state->userData = data;
	state->onStart = onStart;
	state->onUpdate = onUpdate;
	state->onStop = onStop;
	
	return state;
}
