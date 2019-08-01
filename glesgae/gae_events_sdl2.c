#include "gae_events.h"

#include "SDL2/SDL.h"

#include <string.h>

gae_event_system_t* gae_event_system_init(gae_event_system_t* system, void* userDatum)
{
	memset(system, 0, sizeof(gae_event_system_t));
	
	system->userDatum = userDatum;
	
	SDL_Init(SDL_INIT_EVERYTHING);
	
	return system;
}

gae_event_system_t* gae_event_system_update(gae_event_system_t* const system)
{
	SDL_Event event;
	while (0 != SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_MOUSEMOTION: {
				if (0 != system->onMouseEvent) {
					SDL_MouseMotionEvent* motion = (SDL_MouseMotionEvent*)&event;
					gae_pointer_move_event_t moveEvent;
					gae_event_t gaeEvent;
					moveEvent.x = motion->x; 
					moveEvent.y = motion->y;
					gaeEvent.type = GAE_EVENT_MOUSE_MOVE; gaeEvent.event = &moveEvent;
					(*system->onMouseEvent)(system->userDatum, &gaeEvent);
				}
			};
			break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP: {
				if (0 != system->onMouseEvent) {
					SDL_MouseButtonEvent* button = (SDL_MouseButtonEvent*)&event;
					gae_pointer_button_event_t buttonEvent;
					gae_event_t gaeEvent;
					buttonEvent.buttonId = button->which; 
					buttonEvent.isDown = button->type == SDL_MOUSEBUTTONDOWN;
					gaeEvent.type = GAE_EVENT_MOUSE_BUTTON; gaeEvent.event = &buttonEvent;
					(*system->onMouseEvent)(system->userDatum, &gaeEvent);
				}
			};
			break;
			case SDL_MOUSEWHEEL: break;
			case SDL_KEYDOWN:
			case SDL_KEYUP: {
				if (0 != system->onKeyboardEvent) {
					SDL_KeyboardEvent* key = (SDL_KeyboardEvent*)&event;
					gae_keyboard_event_t keyEvent;
					gae_event_t gaeEvent;
					keyEvent.key = key->keysym.scancode; 
					keyEvent.isDown = key->type == SDL_KEYDOWN;
					gaeEvent.type = GAE_EVENT_KEYBOARD;
					gaeEvent.event = &keyEvent;
					(*system->onKeyboardEvent)(system->userDatum, &gaeEvent);
				}
			};
			break;
			case SDL_WINDOWEVENT: break;
			case SDL_QUIT: {
				gae_event_t quitEvent;
				quitEvent.type = GAE_EVENT_QUIT; 
				quitEvent.event = 0;
				(*system->onQuit)(system->userDatum, &quitEvent);
			};
			break;
		}
	}
	
	return system;
}

gae_event_system_t* gae_event_system_destroy(gae_event_system_t* system)
{
	SDL_Quit();
	
	return system;
}
