#include "gae.h"

#include "fishy_area.h"

#include <stdio.h>
#include <stdlib.h>

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#include <emscripten/html5.h>
#include "SDL2/SDL.h"

void emscripten_fullscreen()
{
	emscripten_request_fullscreen(NULL, EM_FALSE);
	SDL_SetWindowFullscreen(gae_system.graphics.window->data, SDL_WINDOW_FULLSCREEN);
	printf("Fullscreening...\n");
}
#endif

static void main_loop();
static int isRunning = 1;

static gae_graphics_texture_t badger;
static gae_graphics_texture_t waterTex;
static gae_rect_t drawRect;

static void OnQuit(void* userDatum, gae_event_t* const event)
{
	(void)(userDatum);
	(void)(event);
	
	isRunning = 0;
}

void fillWaterTexture(water_area_t* area)
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
	
	gae_graphics_texture_fill_from_buffer(&waterTex, gae_system.graphics.context, &texBuffer, area->area.columns, area->area.rows, 24);
}

static void OnMouseEvent(void* userDatum, gae_event_t* event)
{
	(void)(userDatum);
	
	switch (event->type) {
		case GAE_EVENT_MOUSE_BUTTON: {

		};
		break;
		default:
		break;
	}
}

int main(int argc, char** argv)
{
	water_area_t waterArea;
	
	gae_system.event_system = gae_event_system_init(gae_alloc(sizeof(gae_event_system_t)), 0);
	gae_system.event_system->onMouseEvent = OnMouseEvent;
	
	gae_system.graphics.window = gae_graphics_window_init(gae_alloc(sizeof(gae_graphics_window_t)), "Low Rez Fish'n", 0x2FFF0000U, 0x2FFF0000U, 512, 512, 0);
	gae_system.graphics.context = gae_graphics_context_init(gae_alloc(sizeof(gae_graphics_context_t)), gae_system.graphics.window, 0);
	
	gae_graphics_context_set_render_size(gae_system.graphics.context, 64, 64);
	
	gae_system.event_system->onQuit = OnQuit;
	
	gae_graphics_texture_init(&badger);
	gae_graphics_context_texture_load_from_file(gae_system.graphics.context, "data/badger.bmp", &badger);
	drawRect.x = 0; drawRect.y = 0; drawRect.w = 64; drawRect.h = 64;
	
	water_area_init(&waterArea, 64, 64, 0);
	water_area_print(&waterArea);
	fillWaterTexture(&waterArea);
	water_area_destroy(&waterArea);
	
	(void)(argc);
	(void)(argv);
	
#if !defined(__EMSCRIPTEN__)
	while (isRunning)
		main_loop();
	
	gae_graphics_texture_destroy(&badger);
	gae_graphics_window_destroy(gae_system.graphics.window);
	gae_graphics_context_destroy(gae_system.graphics.context);
	gae_event_system_destroy(gae_system.event_system);
	
#else
	emscripten_set_main_loop(main_loop, 0, 1);
#endif
	
	return 0;
}

/* main loop separated for Emscripten to work */
static void main_loop()
{
	gae_event_system_update(gae_system.event_system);
	
	gae_graphics_context_clear(gae_system.graphics.context);
	gae_graphics_context_blit_texture(gae_system.graphics.context, &waterTex, &drawRect, &drawRect);
	gae_graphics_context_blit_texture(gae_system.graphics.context, &badger, &drawRect, &drawRect);
	
	gae_graphics_context_present(gae_system.graphics.context);
	
}
