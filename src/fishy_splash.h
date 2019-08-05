#ifndef _FISHY_SPLASH_H_
#define _FISHY_SPLASH_H_

#include "gae.h"

typedef struct fishy_splash_s {
	gae_graphics_texture_t splash;
	gae_timer_t timer;
} fishy_splash_t;

typedef struct fishy_splash_state_s {
	gae_list_t splashScreens;
} fishy_splash_state_t;

#endif
