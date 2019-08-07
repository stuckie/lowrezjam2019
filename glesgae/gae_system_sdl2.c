#include "gae_system.h"
#include "SDL2/SDL.h"

void gae_system_delay(unsigned int ms)
{
	SDL_Delay((Uint32)ms);
}
