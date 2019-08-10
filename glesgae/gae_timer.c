#include "gae_timer.h"
#include "gae_clock.h"

#include <stdlib.h>

gae_timer_t* gae_timer_init(gae_timer_t* timer, gae_clock_t* const clock)
{
	timer->currentTime = 0.0F;
	timer->deltaTime = 0.0F;
	timer->lastTime = clock->deltaTime;
	timer->scale = 1.0F;

	return timer;
}

gae_timer_t* gae_timer_update(gae_timer_t* timer, gae_clock_t* const clock)
{
	if (0.0F != timer->scale)
		timer->deltaTime = (clock->deltaTime - timer->lastTime) * timer->scale;
	else
		timer->deltaTime = 0.0F;
	
	timer->lastTime = clock->deltaTime;
	timer->currentTime += timer->deltaTime;

	return timer;
}

gae_timer_t* gae_timer_reset(gae_timer_t* timer)
{
	timer->currentTime = 0.0F;
	timer->deltaTime = 0.0F;
	
	return timer;
}

gae_timer_t* gae_timer_destroy(gae_timer_t* timer)
{
	return timer;
}
