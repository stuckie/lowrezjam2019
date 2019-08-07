#include "fishy_lake.h"
#include "fishy_area.h"

#include "gae.h"

typedef struct fishy_lake_state_s {
	gae_graphics_texture_t waterTex;
	gae_rect_t drawRect;
	water_area_t waterArea;
} fishy_lake_state_t;

static void fillWaterTexture(water_area_t* area, gae_graphics_texture_t* waterTex)
{
	gae_buffer_t texBuffer;
	int i = 0;
	int size = area->area.columns * area->area.rows;
	gae_colour_hsv hsv;
	gae_colour_rgba rgba;
	
	gae_buffer_init(&texBuffer, GAE_BUFFER_FIXED, size * 3U);
	
	for (i = 0; i < size; ++i) {
		gae_grid_cell_t* cell = area->area.cells + i;
		water_cell_t* tile = cell->data;
		
		hsv.h = 200;
		hsv.s = 1.0;
		hsv.v = 1.0 - (float)(tile->depth / 10.0);
		
		rgba = gae_colour_hsv_to_rgb(hsv);
		
		texBuffer.data[(i * 3) + 0] = rgba.r;
		texBuffer.data[(i * 3) + 1] = rgba.g;
		texBuffer.data[(i * 3) + 2] = rgba.b;
	}
	
	gae_graphics_texture_fill_from_buffer(waterTex, gae_system.graphics.context, &texBuffer, area->area.columns, area->area.rows, 24);
	
	gae_buffer_destroy(&texBuffer);
}

static int onStart(void* userData)
{
	fishy_lake_state_t* data = userData;
	
	water_area_init(&data->waterArea, 64, 64, 0);
	water_area_print(&data->waterArea);
	fillWaterTexture(&data->waterArea, &data->waterTex);
	water_area_destroy(&data->waterArea);
	
	return 0;
}

static int onUpdate(void* userData)
{
	fishy_lake_state_t* data = userData;
	gae_graphics_context_blit_texture(gae_system.graphics.context, &data->waterTex, &data->drawRect, &data->drawRect);
	
	return 0;
}

static int onStop(void* userData)
{
	fishy_lake_state_t* data = userData;
	
	gae_graphics_texture_destroy(&data->waterTex);
	
	return 0;
}

gae_state_t* fishy_lake_init(gae_state_t* state)
{
	fishy_lake_state_t* data = gae_alloc(sizeof(fishy_lake_state_t));
	
	data->drawRect.x = 0; data->drawRect.y = 0; data->drawRect.w = 64; data->drawRect.h = 64;
	
	state->userData = data;
	state->onStart = onStart;
	state->onUpdate = onUpdate;
	state->onStop = onStop;
	
	return state;
}
