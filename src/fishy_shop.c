#include "fishy_structs.h"

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

static int onUpdate(void* userData)
{
	fishy_shop_t* data = userData;
	
	gae_graphics_context_blit_texture(gae_system.graphics.context, &data->pic, 0, 0);
	
	return GLOBAL.pointer.isDown[0];
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
