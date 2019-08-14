#include "fishy_structs.h"

#include "gae.h"

#define SWING_SPEED 360

enum swing_state
{	swing_start
,	swing_end
};

typedef struct fishy_cast_s {
	gae_timer_t timer;
	
	float angle;
	
	gae_graphics_texture_t ui;
	
	gae_point_2d_t start;
	gae_point_2d_t length;
	gae_point_2d_t end;
	
	enum swing_state state;
} fishy_cast_t;

static int onStart(void* userData)
{
	fishy_cast_t* data = userData;
	
	gae_graphics_texture_init(&data->ui);
	gae_graphics_context_texture_load_from_file(gae_system.graphics.context, "data/cast_ui.bmp", &data->ui);
	
	gae_timer_init(&data->timer, gae_system.main_clock);
	data->timer.scale = SWING_SPEED;
	
	data->angle = 0.0F;
	data->start.x = 21;
	data->start.y = 38;
	data->length.x = data->start.x + 10;
	data->length.y = data->start.y - 10;
	data->end.x = 0;
	data->end.y = 0;
	
	return 0;
}

static int onUpdate(void* userData)
{
	fishy_cast_t* data = userData;
	gae_colour_rgba colour;
	
	if (GLOBAL.pointer.isDown[0])
		data->angle += data->timer.deltaTime;
	else {
	}
	
	gae_timer_update(&data->timer, gae_system.main_clock);
	
	gae_graphics_context_blit_texture(gae_system.graphics.context, &data->ui, 0, 0);
	
	gae_colour_rgba_set_black(colour);
	gae_graphics_context_set_draw_colour(gae_system.graphics.context, &colour);
	data->end = gae_point2d_rotate(&data->start, gae_deg2rad(data->angle), &data->length);
	gae_graphics_context_draw_line(gae_system.graphics.context, &data->start, &data->end);
	
	return 0;
}

static int onStop(void* userData)
{
	fishy_cast_t* data = userData;
	
	gae_graphics_texture_destroy(&data->ui);
	
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