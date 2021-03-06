#include "fishy_timer.h"

#include <math.h>

fishy_timer_t* fishy_timer_init(fishy_timer_t* timer)
{	
	timer->minutes = 0;
	timer->seconds = 0;
	
	gae_timer_init(&timer->timer, gae_system.main_clock);
	timer->timer.scale = -1;
	timer->timer.currentTime = 2 * 60;

	timer->blip = gae_hashstring_calculate(":");
	timer->back = gae_hashstring_calculate("timer");
	
	return timer;
}

fishy_timer_t* fishy_timer_update(fishy_timer_t* timer)
{
	float mins = timer->timer.currentTime / 60;
	gae_timer_update(&timer->timer, gae_system.main_clock);
	timer->minutes = floor(mins);
	timer->seconds = timer->timer.currentTime - (timer->minutes * 60);
	
	return timer;
}

fishy_timer_t* fishy_timer_draw(fishy_timer_t* timer, gae_point_2d_t pos)
{
	int minTens = timer->minutes / 10;
	int minUnits = timer->minutes - minTens;
	
	int secTens = timer->seconds / 10;
	int secUnits = timer->seconds - (secTens * 10);
	
	gae_rect_t rect;
	rect.x = pos.x;
	rect.y = pos.y;

	if (0 > timer->timer.currentTime) return timer;
	
	gae_sprite_sheet_draw(&GLOBAL.sprites, timer->back, &rect);
	rect.x += 2;
	rect.y += 2;
	gae_sprite_sheet_draw(&GLOBAL.sprites, 48 + minTens, &rect);
	rect.x += rect.w + 1;
	gae_sprite_sheet_draw(&GLOBAL.sprites, 48 + minUnits, &rect);
	rect.x += rect.w;
	gae_sprite_sheet_draw(&GLOBAL.sprites, timer->blip, &rect);
	rect.x += rect.w;
	gae_sprite_sheet_draw(&GLOBAL.sprites, 48 + secTens, &rect);
	rect.x += rect.w + 1;
	gae_sprite_sheet_draw(&GLOBAL.sprites, 48 + secUnits, &rect);
	
	return timer;
}

fishy_timer_t* fishy_timer_paused(fishy_timer_t* timer)
{
	timer->timer.lastTime = gae_system.main_clock->deltaTime;
	timer->timer.deltaTime = 0;

	return timer;
}
