#include "gae.h"

#include "fishy_splash.h"
#include "fishy_area.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct fish_global_s {
	gae_timer_t frameCap;
	float fps;
	float ticksPerFrame;
} fish_global_t;

fish_global_t GLOBAL;

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
	
	gae_buffer_destroy(&texBuffer);
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

gae_state_t splash;
gae_sprite_sheet_t sprites;
gae_rect_t boatRect;
int main(int argc, char** argv)
{
	water_area_t waterArea;
	gae_json_document_t jsDoc;
	
	gae_system.main_clock = gae_clock_init(gae_alloc(sizeof(gae_clock_t)));
	gae_timer_init(&GLOBAL.frameCap, gae_system.main_clock);
    GLOBAL.fps = 1.0F / 60.0F;
    GLOBAL.ticksPerFrame = GLOBAL.fps * 1000.0F;
	
	gae_system.event_system = gae_event_system_init(gae_alloc(sizeof(gae_event_system_t)), 0);
	gae_system.event_system->onMouseEvent = OnMouseEvent;
	
	gae_system.graphics.window = gae_graphics_window_init(gae_alloc(sizeof(gae_graphics_window_t)), "Low Rez Fish'n", 0x2FFF0000U, 0x2FFF0000U, 512, 512, 0);
	gae_system.graphics.context = gae_graphics_context_init(gae_alloc(sizeof(gae_graphics_context_t)), gae_system.graphics.window, 0);
	
	gae_graphics_context_set_render_size(gae_system.graphics.context, 64, 64);
	
	gae_system.event_system->onQuit = OnQuit;
	
	drawRect.x = 0; drawRect.y = 0; drawRect.w = 64; drawRect.h = 64;
	boatRect.x = 16, boatRect.y = 16;
	
	water_area_init(&waterArea, 64, 64, 0);
	water_area_print(&waterArea);
	fillWaterTexture(&waterArea);
	water_area_destroy(&waterArea);
	
	gae_json_document_init(&jsDoc, "data/sprites.json");
	gae_json_document_parse(&jsDoc);
	gae_sprite_sheet_init(&sprites, &jsDoc);
	gae_json_document_destroy(&jsDoc);
	
	fishy_splash_init(&splash);
	
	(void)(argc);
	(void)(argv);
	
	(*splash.onStart)(splash.userData);
	
#if !defined(__EMSCRIPTEN__)
	while (isRunning)
		main_loop();
	
	(*splash.onStop)(splash.userData);
	fishy_splash_destroy(&splash);
	
	gae_sprite_sheet_destroy(&sprites);
	gae_graphics_texture_destroy(&waterTex);
	gae_graphics_window_destroy(gae_system.graphics.window);
	gae_graphics_context_destroy(gae_system.graphics.context);
	gae_event_system_destroy(gae_system.event_system);
	
	gae_free(gae_system.event_system);
	gae_free(gae_system.graphics.window);
	gae_free(gae_system.graphics.context);
	gae_free(gae_system.main_clock);
	
#else
	emscripten_set_main_loop(main_loop, 0, 1);
#endif
	
	return 0;
}

/* main loop separated for Emscripten to work */
static void main_loop()
{
	gae_timer_update(&GLOBAL.frameCap, gae_system.main_clock);
	gae_timer_reset(&GLOBAL.frameCap);
	
	gae_clock_update(gae_system.main_clock);
	
	gae_event_system_update(gae_system.event_system);
	
	gae_graphics_context_clear(gae_system.graphics.context);
	gae_graphics_context_blit_texture(gae_system.graphics.context, &waterTex, &drawRect, &drawRect);
	(*splash.onUpdate)(splash.userData);
	
	gae_graphics_context_present(gae_system.graphics.context);
	
	gae_timer_update(&GLOBAL.frameCap, gae_system.main_clock);
	
	gae_system_delay(GLOBAL.ticksPerFrame - GLOBAL.frameCap.currentTime);
}
