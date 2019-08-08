#include "fishy_structs.h"
#include "fishy_lake.h"
#include "fishy_area.h"

#include "gae.h"

typedef struct gae_camera_s {
	gae_rect_t view;
	gae_rect_t port;
} gae_camera_t;

typedef struct fishy_lake_state_s {
	gae_graphics_texture_t waterTex;
	gae_graphics_texture_t ui;
	gae_rect_t uiRect;
	gae_rect_t drawRect;
	water_area_t waterArea;
	gae_camera_t camera;
	gae_camera_t minimap;
	gae_sprite_sheet_t sprites;
	gae_rect_t boatRect;
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
		hsv.v = 1.0 - (float)(tile->depth / 10.0F);
		
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
	gae_json_document_t jsDoc;
	
	water_area_init(&data->waterArea, 64, 64, 0);
	water_area_print(&data->waterArea);
	fillWaterTexture(&data->waterArea, &data->waterTex);
	water_area_destroy(&data->waterArea);
	
	gae_graphics_context_texture_load_from_file(gae_system.graphics.context, "data/lake_ui.bmp", &data->ui);
	
	gae_json_document_init(&jsDoc, "data/sprites.json");
	gae_json_document_parse(&jsDoc);
	gae_sprite_sheet_init(&data->sprites, &jsDoc);
	gae_json_document_destroy(&jsDoc);
	
	data->camera.view.x = 0;
	data->camera.view.y = 0;
	data->camera.view.w = 8;
	data->camera.view.h = 8;
	
	data->camera.port.x = 0;
	data->camera.port.y = 0;
	data->camera.port.w = 64;
	data->camera.port.h = 64;
	
	data->minimap.view.x = 0;
	data->minimap.view.y = 0;
	data->minimap.view.w = 13;
	data->minimap.view.h = 13;
	
	data->minimap.port.x = 50;
	data->minimap.port.y = 50;
	data->minimap.port.w = 13;
	data->minimap.port.h = 13;
	
	data->uiRect.x = 0;
	data->uiRect.y = 48;
	data->uiRect.w = 64;
	data->uiRect.h = 16;
	
	data->boatRect.x = 0;
	data->boatRect.y = 0;
	
	return 0;
}
#include "SDL2/SDL.h"
static int onUpdate(void* userData)
{
	fishy_lake_state_t* data = userData;
	
	if (1 == GLOBAL.keyboard.down[SDL_SCANCODE_LEFT]) { data->camera.view.x--; data->minimap.view.x--; }
	else if (1 == GLOBAL.keyboard.down[SDL_SCANCODE_RIGHT]) { data->camera.view.x++; data->minimap.view.x++; }
	if (1 == GLOBAL.keyboard.down[SDL_SCANCODE_UP]) { data->camera.view.y--; data->minimap.view.y--; }
	else if (1 == GLOBAL.keyboard.down[SDL_SCANCODE_DOWN]) { data->camera.view.y++; data->minimap.view.y++; }
	if (1 == GLOBAL.keyboard.down[SDL_SCANCODE_A]) { data->camera.view.h--; data->camera.view.w--; }
	else if (1 == GLOBAL.keyboard.down[SDL_SCANCODE_Z]) { data->camera.view.h++; data->camera.view.w++; }
	
	gae_graphics_context_blit_texture(gae_system.graphics.context, &data->waterTex, &data->camera.view, &data->camera.port);
	gae_sprite_sheet_draw(&data->sprites, gae_hashstring_calculate("boat"), &data->boatRect);
	gae_graphics_context_blit_texture(gae_system.graphics.context, &data->ui, 0, &data->uiRect);
	gae_graphics_context_blit_texture(gae_system.graphics.context, &data->waterTex, &data->minimap.view, &data->minimap.port);
	
	return 0;
}

static int onStop(void* userData)
{
	fishy_lake_state_t* data = userData;
	
	gae_graphics_texture_destroy(&data->waterTex);
	gae_graphics_texture_destroy(&data->ui);
	gae_sprite_sheet_destroy(&data->sprites);
	
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
