#include "fishy_splash.h"

#include "gae.h"

typedef struct fishy_splash_s {
	gae_graphics_texture_t texture;
	gae_timer_t timer;
	float startTime;
} fishy_splash_t;

typedef struct fishy_splash_state_s {
	gae_list_t splashScreens;
	gae_rect_t drawRect;
} fishy_splash_state_t;

static void resetTime(gae_list_node_t* node)
{
	fishy_splash_t* data = node->data;
	gae_timer_reset(&data->timer);
	data->timer.currentTime = data->startTime;
	data->timer.scale = -1.0F;
}

static int onStart(void* userData)
{
	fishy_splash_state_t* data = userData;
	gae_list_foreach(&data->splashScreens, resetTime);
	
	return 0;
}

static int onUpdate(void* userData)
{
	fishy_splash_state_t* data = userData;
	gae_list_node_t* node = data->splashScreens.start;
	while (0 != node) {
		fishy_splash_t* splash = node->data;
		if (0.0F < splash->timer.currentTime) {
			gae_timer_update(&splash->timer, gae_system.main_clock);
			gae_graphics_context_blit_texture(gae_system.graphics.context, &splash->texture, &data->drawRect, &data->drawRect);
			return 0;
		}
		node = node->next;
	}
	
	return 1;
}

static int onStop(void* userData)
{
	(void)(userData);
	
	return 0;
}

static void addSplash(gae_list_t* list, const char* path, float time)
{
	gae_list_node_t* node = gae_alloc(sizeof(gae_list_node_t));
	fishy_splash_t* splash = gae_alloc(sizeof(fishy_splash_t));
	
	gae_graphics_texture_init(&splash->texture);
	gae_graphics_context_texture_load_from_file(gae_system.graphics.context, path, &splash->texture);
	
	gae_timer_init(&splash->timer, gae_system.main_clock);
	splash->startTime = time;
	splash->timer.currentTime = time;
	splash->timer.scale = -1.0F;
	
	node->data = splash;
	
	gae_list_add(list, node);
}

gae_state_t* fishy_splash_init(gae_state_t* state)
{
	fishy_splash_state_t* data = gae_alloc(sizeof(fishy_splash_t));
	gae_list_init(&data->splashScreens);
	
	addSplash(&data->splashScreens, "data/badger.bmp", 2.0F);
	
	data->drawRect.x = 0; data->drawRect.y = 0; data->drawRect.w = 64; data->drawRect.h = 64;
	
	state->userData = data;
	state->onStart = onStart;
	state->onUpdate = onUpdate;
	state->onStop = onStop;
	
	return state;
}

static void deleteSplash(gae_list_node_t* node)
{
	fishy_splash_t* data = node->data;
	
	gae_graphics_texture_destroy(&data->texture);
	gae_timer_destroy(&data->timer);
}

gae_state_t* fishy_splash_destroy(gae_state_t* state)
{
	fishy_splash_state_t* data = state->userData;
	gae_list_foreach(&data->splashScreens, deleteSplash);
	gae_list_destroy(&data->splashScreens);
	gae_free(state->userData);
	
	return state;
}
