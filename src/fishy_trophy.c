#include "fishy_structs.h"
#include "fishy_timer.h"

#include "gae.h"

#include <math.h>
#include <stdio.h>

typedef struct fishy_trophy_s {
	gae_graphics_texture_t star;
	gae_graphics_context_blit_params_t params;
	int buttonDown;
	float time;
	
	gae_hashstring ids[16];
	gae_rect_t itemRect[16];
	
} fishy_trophy_t;

static int onStart(void* userData)
{
	fishy_trophy_t* data = userData;
	int i;
	
	data->buttonDown = GLOBAL.pointer.isDown[0];
	data->time = 0;
	gae_graphics_context_texture_load_from_file(gae_system.graphics.context, "data/star.bmp", &data->star);
	
	for (i = 0; i < 16; ++i) {
		char c[12];
		sprintf(c, "%d", i); 
		data->ids[i] = gae_hashstring_calculate(c);
		data->itemRect[i].x = (i * 16) % 64;
		data->itemRect[i].y = ((i / 4)) * 16;
	}
	
	data->params.texture = &data->star;
	data->params.srcRect = 0;
	data->params.dstRect = 0;
	data->params.origin = 0;
	data->params.degrees = 0;
	data->params.flip = GAE_TEXTURE_FLIP_NONE;
	
	return 0;
}

static int onUpdate(void* userData)
{
	fishy_trophy_t* data = userData;
	int next = (0 == data->buttonDown && GLOBAL.pointer.isDown[0]);
	gae_colour_rgba colour;
	int i;
	
	data->time += 0.1;
	
	gae_colour_rgba_set_cyan(colour);
	gae_graphics_context_set_draw_colour(gae_system.graphics.context, &colour);
	gae_graphics_context_draw_filled_rect(gae_system.graphics.context, 0);
	
	for (i = 0; i < 16; ++i) {
		gae_rect_t rect = data->itemRect[i];
		gae_hashstring id = data->ids[i];
		int toShow = GLOBAL.trophies[i];
		
		rect.y += sin(data->time + i) * 2;
		
		if (toShow) { gae_colour_rgba_set_white(colour); }
		else { gae_colour_rgba_set_black(colour); }
		
		if (GLOBAL.itemCatch == id) {
			gae_colour_hsv hsv;
			gae_colour_rgba rgb;
			gae_rect_t starRect = rect;
			
			gae_colour_rgba_set_white(colour);
			
			hsv.h = fmod(data->time, 360);
			hsv.s = 1.0F;
			hsv.v = 1.0F;
			
			rgb = gae_colour_hsv_to_rgb(hsv);
			gae_graphics_context_texture_colour(gae_system.graphics.context, &data->star, &rgb);
			
			starRect.x -= 4;
			starRect.y -= 4;
			starRect.w = 24;
			starRect.h = 24;
			data->params.dstRect = &starRect;
			gae_graphics_context_blit_texture_params(gae_system.graphics.context, &data->params);
		}
		
		gae_graphics_context_texture_colour(gae_system.graphics.context, &GLOBAL.items.texture, &colour);
		gae_sprite_sheet_draw(&GLOBAL.items, id, &rect);
	}
	
	data->buttonDown = GLOBAL.pointer.isDown[0];
	
	return next;
}

static int onStop(void* userData)
{
	fishy_trophy_t* data = userData;
	(void)(data);

	fishy_timer_paused(&GLOBAL.time);
	
	return 0;
}

gae_state_t* fishy_trophy_init(gae_state_t* state)
{
	fishy_trophy_t* data = gae_alloc(sizeof(fishy_trophy_t));
	
	state->userData = data;
	state->onStart = onStart;
	state->onUpdate = onUpdate;
	state->onStop = onStop;
	
	return state;
}
