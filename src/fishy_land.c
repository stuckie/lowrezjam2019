#include "fishy_structs.h"
#include "fishy_timer.h"

#include "gae.h"

#include <math.h>
#include <stdio.h>

typedef struct fishy_land_s {
	gae_graphics_texture_t star;
	gae_graphics_context_blit_params_t params;
	int buttonDown;
	float time;
	gae_rect_t itemRect;
} fishy_land_t;

int value;
static int onStart(void* userData)
{
	fishy_land_t* data = userData;
	gae_colour_rgba colour;
	gae_colour_rgba_set_white(colour);
	gae_graphics_context_texture_colour(gae_system.graphics.context, &GLOBAL.items.texture, &colour);
	
	data->buttonDown = GLOBAL.pointer.isDown[GAE_MOUSE_BUTTON_ANY];
	gae_graphics_context_texture_load_from_file(gae_system.graphics.context, "data/star.bmp", &data->star);
	data->params.texture = &data->star;
	data->params.srcRect = 0;
	data->params.dstRect = 0;
	data->params.origin = 0;
	data->params.degrees = 0;
	data->params.flip = GAE_TEXTURE_FLIP_NONE;
	
	data->itemRect.x = 24;
	data->itemRect.y = 24;
	data->time = 0.0F;
	
	value = 0;
	
	return 0;
}

static int onUpdate(void* userData)
{
	fishy_land_t* data = userData;
	int next = (0 == data->buttonDown && GLOBAL.pointer.isDown[GAE_MOUSE_BUTTON_ANY]);
	gae_colour_rgba colour;
	gae_point_2d_t origin;
	gae_rect_t draw = data->itemRect;
	
	origin.x = 32;
	origin.y = 32;
	
	data->time += 0.1;
	data->params.origin = &origin;
	data->params.degrees = data->time;
	
	gae_colour_rgba_set_cyan(colour);
	gae_graphics_context_set_draw_colour(gae_system.graphics.context, &colour);
	gae_graphics_context_draw_filled_rect(gae_system.graphics.context, 0);
	
	gae_colour_rgba_set_red(colour);
	gae_graphics_context_texture_colour(gae_system.graphics.context, &data->star, &colour);
	gae_graphics_context_blit_texture_params(gae_system.graphics.context, &data->params);
	
	gae_colour_rgba_set_yellow(colour);
	gae_graphics_context_texture_colour(gae_system.graphics.context, &data->star, &colour);
	data->params.degrees *= 1.333;
	gae_graphics_context_blit_texture_params(gae_system.graphics.context, &data->params);
	
	gae_colour_rgba_set_green(colour);
	gae_graphics_context_texture_colour(gae_system.graphics.context, &data->star, &colour);
	data->params.degrees *= 1.333;
	gae_graphics_context_blit_texture_params(gae_system.graphics.context, &data->params);
	
	gae_colour_rgba_set_blue(colour);
	gae_graphics_context_texture_colour(gae_system.graphics.context, &data->star, &colour);
	data->params.degrees *= 1.333;
	gae_graphics_context_blit_texture_params(gae_system.graphics.context, &data->params);
	
	draw.y += sin(data->time) * 4;
	gae_sprite_sheet_draw(&GLOBAL.items, GLOBAL.itemCatch, &draw);
	
	data->buttonDown = GLOBAL.pointer.isDown[GAE_MOUSE_BUTTON_ANY];

	return next;
}

static int onStop(void* userData)
{
	fishy_land_t* data = userData;
	
	GLOBAL.tile->fish = -1;
	gae_graphics_texture_destroy(&data->star);
	fishy_timer_paused(&GLOBAL.time);
	
	return 0;
}

gae_state_t* fishy_land_init(gae_state_t* state)
{
	fishy_land_t* data = gae_alloc(sizeof(fishy_land_t));
	
	state->userData = data;
	state->onStart = onStart;
	state->onUpdate = onUpdate;
	state->onStop = onStop;
	
	return state;
}
