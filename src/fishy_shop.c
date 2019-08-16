#include "fishy_structs.h"
#include "fishy_timer.h"

#include "gae.h"

typedef struct fishy_shop_s {
	gae_graphics_texture_t pic;
} fishy_shop_t;

static int onStart(void* userData)
{
	fishy_shop_t* data = userData;
	
	gae_graphics_context_texture_load_from_file(gae_system.graphics.context, "data/start.bmp", &data->pic);
	
	return 0;
}

static void initGame()
{
	int i;
	
	for (i = 0; i < 16; ++i)
		GLOBAL.trophies[i] = 0;
	
	fishy_timer_init(&GLOBAL.time);
}

static int onUpdate(void* userData)
{
	fishy_shop_t* data = userData;
	int next = GLOBAL.pointer.isDown[0];
	
	gae_graphics_context_blit_texture(gae_system.graphics.context, &data->pic, 0, 0);
	
	if (next) initGame();
	
	return next;
}

static int onStop(void* userData)
{
	fishy_shop_t* data = userData;
	
	gae_graphics_texture_destroy(&data->pic);
	
	return 0;
}

gae_state_t* fishy_shop_init(gae_state_t* state)
{
	fishy_shop_t* data = gae_alloc(sizeof(fishy_shop_t));
	
	state->userData = data;
	state->onStart = onStart;
	state->onUpdate = onUpdate;
	state->onStop = onStop;
	
	return state;
}
