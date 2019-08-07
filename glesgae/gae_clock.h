#ifndef _GAE_CLOCK_H_
#define _GAE_CLOCK_H_

typedef struct gae_clock_s {
	float deltaTime;
	float currentTime;
	void* platformData;
} gae_clock_t;

gae_clock_t* gae_clock_init(gae_clock_t* clock);
gae_clock_t* gae_clock_reset(gae_clock_t* clock);
gae_clock_t* gae_clock_update(gae_clock_t* clock);
gae_clock_t* gae_clock_pause(gae_clock_t* clock);
gae_clock_t* gae_clock_resume(gae_clock_t* clock);
gae_clock_t* gae_clock_destroy(gae_clock_t* clock);

#endif
