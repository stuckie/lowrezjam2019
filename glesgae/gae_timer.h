#ifndef _GAE_TIMER_H_
#define _GAE_TIMER_H_

struct gae_clock_s;

typedef struct gae_timer_s {
	float lastTime;
	float currentTime;
	float deltaTime;
	float scale;
} gae_timer_t;

gae_timer_t* gae_timer_init(gae_timer_t* timer, struct gae_clock_s* const clock);
gae_timer_t* gae_timer_reset(gae_timer_t* timer);
gae_timer_t* gae_timer_update(gae_timer_t* timer, struct gae_clock_s* const clock);
gae_timer_t* gae_timer_destroy(gae_timer_t* timer);

#endif
