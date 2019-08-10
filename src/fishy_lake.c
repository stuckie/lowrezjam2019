#include "fishy_structs.h"
#include "fishy_lake.h"
#include "fishy_area.h"
#include "fishy_cast.h"

#include "gae.h"

typedef struct fishy_lake_state_s {
	gae_sprite_sheet_t sprites;
	gae_graphics_texture_t waterTex;
	gae_graphics_texture_t ui;
	
	gae_rect_t uiRect;
	gae_rect_t drawRect;
	gae_rect_t boatRect;
	
	gae_camera_t camera;
	gae_camera_t minimap;
	
	gae_point_2d_t boatDest;
	water_area_t waterArea;
	
	gae_button_t cast;
	gae_button_t shop;
	
	int quit;
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
		hsv.s = 0.0;
		hsv.v = (float)(tile->depth / 10.0F);
		
		rgba = gae_colour_hsv_to_rgb(hsv);
		
		texBuffer.data[(i * 3) + 0] = rgba.r;
		texBuffer.data[(i * 3) + 1] = rgba.g;
		texBuffer.data[(i * 3) + 2] = rgba.b;
	}
	
	gae_graphics_texture_fill_from_buffer(waterTex, gae_system.graphics.context, &texBuffer, area->area.columns, area->area.rows, 24);
	
	gae_buffer_destroy(&texBuffer);
}

static void setupCameras(fishy_lake_state_t* data)
{
	gae_point_2d_t mainView, mainPort, miniView, miniPort;
	mainView.x = 8; mainView.y = 8;		/* width/height of view into texture */
	mainPort.x = 64; mainPort.y = 64;	/* width/height of port on screen */
	
	miniView.x = 13; miniView.y = 13;	/* width/height of view into texture */
	miniPort.x = 13; miniPort.y = 13;	/* width/height of port on screen */
	
	gae_camera_init(&data->camera, mainView, mainPort);
	gae_camera_init(&data->minimap, miniView, miniPort);
	
	/* Minimap is only in the bottom right corner of the ui */
	data->minimap.port.x = 50; data->minimap.port.y = 50;
}

static void onCastButton(void* userData)
{
	gae_state_t castState;
	
	fishy_cast_init(&castState);
	if (0 != castState.onStart) (*castState.onStart)(castState.userData);
	gae_stack_push(&GLOBAL.stateStack, &castState);
	
	(void)(userData);
}

static void onShopButton(void* userData)
{
	fishy_lake_state_t* data = userData;
	data->quit = 1;
}

static void makeButton(gae_button_t* button, const char* jsonDef, int x, int y, gae_button_callback_t onReleased, void* userData)
{
	gae_point_2d_t pos;
	pos.x = x;
	pos.y = y;
	
	gae_button_init(button, jsonDef, &pos, userData);
	button->onReleased = onReleased;
}

static int onStart(void* userData)
{
	fishy_lake_state_t* data = userData;
	gae_json_document_t jsDoc;
	
	water_area_init(&data->waterArea, 64, 64, 0);
	/*water_area_print(&data->waterArea);*/
	fillWaterTexture(&data->waterArea, &data->waterTex);
	water_area_destroy(&data->waterArea);
	
	gae_graphics_context_texture_load_from_file(gae_system.graphics.context, "data/lake_ui.bmp", &data->ui);
	
	gae_json_document_init(&jsDoc, "data/sprites.json");
	gae_json_document_parse(&jsDoc);
	gae_sprite_sheet_init(&data->sprites, &jsDoc);
	gae_json_document_destroy(&jsDoc);
	
	setupCameras(data);
	
	data->uiRect.x = 0;
	data->uiRect.y = 48;
	data->uiRect.w = 64;
	data->uiRect.h = 16;
	
	data->boatRect.x = 0;
	data->boatRect.y = 0;
	data->boatDest.x = 32;
	data->boatDest.y = 32;
	
	makeButton(&data->cast, "data/cast-button.json", 34, 50, onCastButton, data);
	makeButton(&data->shop, "data/shop-button.json", 1, 50, onShopButton, data);
	
	data->quit = 0;
	
	return 0;
}
#include "SDL2/SDL.h"
static int onUpdate(void* userData)
{
	fishy_lake_state_t* data = userData;
	gae_point_2d_t pointer;
	
	if (1 == data->quit) return 1;
	
	if (1 == GLOBAL.keyboard.down[SDL_SCANCODE_LEFT]) { data->camera.view.x--; data->minimap.view.x--; }
	else if (1 == GLOBAL.keyboard.down[SDL_SCANCODE_RIGHT]) { data->camera.view.x++; data->minimap.view.x++; }
	if (1 == GLOBAL.keyboard.down[SDL_SCANCODE_UP]) { data->camera.view.y--; data->minimap.view.y--; }
	else if (1 == GLOBAL.keyboard.down[SDL_SCANCODE_DOWN]) { data->camera.view.y++; data->minimap.view.y++; }
	if (1 == GLOBAL.keyboard.down[SDL_SCANCODE_A]) { data->camera.view.h--; data->camera.view.w--; }
	else if (1 == GLOBAL.keyboard.down[SDL_SCANCODE_Z]) { data->camera.view.h++; data->camera.view.w++; }
	
	if (1 == GLOBAL.pointer.isDown[0]) { 
		data->boatDest.x = GLOBAL.pointer.x;
		data->boatDest.y = GLOBAL.pointer.y;
	}
	
	/*
	 * Thoughts on movement...
	 * Click to move boat around on screen.
	 * Boat gets towards last tile on one side, move boat to centre and move camera with it
	 */
	pointer.x = GLOBAL.pointer.x;
	pointer.y = GLOBAL.pointer.y;
	
	data->boatRect.x = gae_lerp(data->boatRect.x, data->boatDest.x, 0.1);
	data->boatRect.y = gae_lerp(data->boatRect.y, data->boatDest.y, 0.1);
	
	data->camera.view.x = gae_lerp(data->camera.view.x, data->boatRect.x - 16, 0.1);
	data->camera.view.y = gae_lerp(data->camera.view.y, data->boatRect.y - 16, 0.1);
	
	gae_graphics_context_blit_texture(gae_system.graphics.context, &data->waterTex, &data->camera.view, &data->camera.port);
	gae_sprite_sheet_draw(&data->sprites, gae_hashstring_calculate("boat"), &data->boatRect);
	gae_graphics_context_blit_texture(gae_system.graphics.context, &data->ui, 0, &data->uiRect);
	gae_button_update(&data->shop, &pointer, GLOBAL.pointer.isDown[0]);
	gae_button_update(&data->cast, &pointer, GLOBAL.pointer.isDown[0]);
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
