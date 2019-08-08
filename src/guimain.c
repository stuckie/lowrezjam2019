#include "gae.h"

#include "fishy_structs.h"
#include "fishy_splash.h"
#include "fishy_lake.h"

#include <stdio.h>
#include <stdlib.h>

fish_global_t GLOBAL;

static void OnKeyboardEvent(void* userDatum, gae_event_t* event)
{
	gae_keyboard_event_t* key = event->event;
	fish_global_t* data = userDatum;
	
	data->keyboard.down[key->key] = key->isDown;
}

static void OnMouseEvent(void* userDatum, gae_event_t* event)
{
	fish_global_t* data = userDatum;
	
	switch (event->type) {
		case GAE_EVENT_MOUSE_MOVE: {
			gae_pointer_move_event_t* motion = event->event;
			data->pointer.x = motion->x;
			data->pointer.y = motion->y;
		};
		break;
		case GAE_EVENT_MOUSE_BUTTON: {
			gae_pointer_button_event_t* button = event->event;
			data->pointer.isDown[button->buttonId] = button->isDown;
		};
		break;
		default:
		break;
	}
}

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

static void OnQuit(void* userDatum, gae_event_t* const event)
{
	(void)(userDatum);
	(void)(event);
	
	isRunning = 0;
}

int main(int argc, char** argv)
{
	gae_state_t splash;
	gae_state_t lake;
	gae_state_t* current;
	
	gae_system.main_clock = gae_clock_init(gae_alloc(sizeof(gae_clock_t)));
	gae_timer_init(&GLOBAL.framerate.cap, gae_system.main_clock);
    GLOBAL.framerate.fps = 1.0F / 60.0F;
    GLOBAL.framerate.ticksPerFrame = GLOBAL.framerate.fps * 1000.0F;
	
	gae_system.event_system = gae_event_system_init(gae_alloc(sizeof(gae_event_system_t)), &GLOBAL);
	gae_system.event_system->onMouseEvent = OnMouseEvent;
	
	gae_system.graphics.window = gae_graphics_window_init(gae_alloc(sizeof(gae_graphics_window_t)), "Low Rez Fish'n", 0x2FFF0000U, 0x2FFF0000U, 512, 512, 0);
	gae_system.graphics.context = gae_graphics_context_init(gae_alloc(sizeof(gae_graphics_context_t)), gae_system.graphics.window, 0);
	
	gae_graphics_context_set_render_size(gae_system.graphics.context, 64, 64);
	
	gae_system.event_system->onMouseEvent = OnMouseEvent;
	gae_system.event_system->onKeyboardEvent = OnKeyboardEvent;
	gae_system.event_system->onQuit = OnQuit;
	
	fishy_lake_init(&lake);
	fishy_splash_init(&splash);
	gae_stack_init(&GLOBAL.stateStack, sizeof(gae_state_t));
	gae_stack_push(&GLOBAL.stateStack, &lake);
	gae_stack_push(&GLOBAL.stateStack, &splash);
	
	(void)(argc);
	(void)(argv);
	
	current = gae_stack_peek(&GLOBAL.stateStack);
	(*current->onStart)(current->userData);
	
#if !defined(__EMSCRIPTEN__)
	while (isRunning)
		main_loop();
	
	current = gae_stack_peek(&GLOBAL.stateStack);
	if (0 != current)(*current->onStop)(current->userData);
	
	gae_graphics_window_destroy(gae_system.graphics.window);
	gae_graphics_context_destroy(gae_system.graphics.context);
	gae_event_system_destroy(gae_system.event_system);
	
	gae_free(gae_system.event_system);
	gae_free(gae_system.graphics.window);
	gae_free(gae_system.graphics.context);
	gae_free(gae_system.main_clock);
	
#else
	emscripten_set_main_loop(main_loop, 60, 1);
#endif
	
	return 0;
}

/* main loop separated for Emscripten to work */
static void main_loop()
{
	gae_state_t* state = gae_stack_peek(&GLOBAL.stateStack);
	if (0 == state) {
		isRunning = 0;
		return;
	}
	
	gae_timer_update(&GLOBAL.framerate.cap, gae_system.main_clock);
	gae_timer_reset(&GLOBAL.framerate.cap);
	
	gae_clock_update(gae_system.main_clock);
	
	gae_event_system_update(gae_system.event_system);
	
	gae_graphics_context_clear(gae_system.graphics.context);
	if (0 != (*state->onUpdate)(state->userData)) {
		(*state->onStop)(state->userData);
		gae_stack_pop(&GLOBAL.stateStack);
		state = gae_stack_peek(&GLOBAL.stateStack);
		if (0 != state) (*state->onStart)(state->userData);
	}
	
	gae_graphics_context_present(gae_system.graphics.context);
	
	gae_timer_update(&GLOBAL.framerate.cap, gae_system.main_clock);
	
#ifndef __EMSCRIPTEN__
	gae_system_delay(gae_max(0, GLOBAL.framerate.ticksPerFrame - GLOBAL.framerate.cap.currentTime));
#endif
}
