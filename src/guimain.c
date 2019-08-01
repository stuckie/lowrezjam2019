#include "gae.h"

#include <stdio.h>
#include <stdlib.h>

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#endif

static void main_loop();
static int isRunning = 1;
static gae_graphics_texture_t badger;
static gae_rect_t drawRect;

static void OnQuit(void* userDatum, gae_event_t* const event)
{
	(void)(userDatum);
	(void)(event);
	
	isRunning = 0;
}

int main(int argc, char** argv)
{
	gae_system.event_system = gae_event_system_init(gae_alloc(sizeof(gae_event_system_t)), 0);

	gae_system.graphics.window = gae_graphics_window_init(gae_alloc(sizeof(gae_graphics_window_t)), "Here Fishy Fishy!", 0x2FFF0000U, 0x2FFF0000U, 512, 512, 0);
	gae_system.graphics.context = gae_graphics_context_init(gae_alloc(sizeof(gae_graphics_context_t)), gae_system.graphics.window, 0);

	gae_graphics_context_set_render_size(gae_system.graphics.context, 64, 64);

	gae_system.event_system->onQuit = OnQuit;

	gae_graphics_texture_init(&badger);
	gae_graphics_context_texture_load_from_file(gae_system.graphics.context, "data/badger.bmp", &badger);
	drawRect.x = 0; drawRect.y = 0; drawRect.w = 64; drawRect.h = 64;
	
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
	emscripten_set_main_loop(main_loop, 60, 1);
#endif
	
	return 0;
}

/* main loop separated for Emscripten to work */
static void main_loop()
{
	gae_event_system_update(gae_system.event_system);

	gae_graphics_context_blit_texture(gae_system.graphics.context, &badger, &drawRect, &drawRect);
	gae_graphics_context_present(gae_system.graphics.context);
}
