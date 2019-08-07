#include "gae_clock.h"
#include "gae_memory.h"

#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif

#include <stdlib.h>
#include <time.h>

typedef struct gae_clock_linux_s {
	double startTime;
	double currentTime;
	double currentDelta;
	double pausedTime;
} gae_clock_linux_t;

double getCurrentTime(const struct timespec* time);

gae_clock_t* gae_clock_init(gae_clock_t* clock)
{
	gae_clock_linux_t* linuxClock = gae_alloc(sizeof(gae_clock_linux_t));
	
	struct timespec time;
	time.tv_sec = -1;
	time.tv_nsec = 0;
	
	clock->deltaTime = 0.0F;
	clock->currentTime = 0.0F;
	clock->platformData = linuxClock;
	
	clock_gettime(CLOCK_MONOTONIC, &time);
	linuxClock->startTime = getCurrentTime(&time);
	linuxClock->currentTime = linuxClock->startTime;
	linuxClock->currentDelta = 0.0;
	linuxClock->pausedTime = 0.0;
	
	return clock;
}

gae_clock_t* gae_clock_destroy(gae_clock_t* clock) {
	gae_free(clock->platformData);
	clock->platformData = 0;
	return clock;
}

gae_clock_t* gae_clock_reset(gae_clock_t* clock) {
	gae_clock_linux_t* linuxClock = clock->platformData;
	
	struct timespec time;
	time.tv_sec = -1;
	time.tv_nsec = 0;
	
	clock_gettime(CLOCK_MONOTONIC, &time);
	linuxClock->startTime = getCurrentTime(&time);
	linuxClock->currentTime = linuxClock->startTime;
	
	return clock;
}

gae_clock_t* gae_clock_update(gae_clock_t* clock) {
	gae_clock_linux_t* linuxClock = clock->platformData;
	
	struct timespec time;
	time.tv_sec = -1;
	time.tv_nsec = 0;
	
	clock_gettime(CLOCK_MONOTONIC, &time);
	linuxClock->currentTime = getCurrentTime(&time);
	linuxClock->currentDelta = linuxClock->currentTime - linuxClock->startTime;
	
	clock->deltaTime = (float)linuxClock->currentDelta;
	clock->currentTime = (float)linuxClock->currentTime;
	
	return clock;
}

gae_clock_t* gae_clock_pause(gae_clock_t* clock) {
	gae_clock_linux_t* linuxClock = clock->platformData;
	
	struct timespec time;
	time.tv_sec = -1;
	time.tv_nsec = 0;
	
	clock_gettime(CLOCK_MONOTONIC, &time);
	linuxClock->pausedTime = getCurrentTime(&time);
	
	
	return clock;
}

gae_clock_t* gae_clock_resume(gae_clock_t* clock) {
	gae_clock_linux_t* linuxClock = clock->platformData;
	
	struct timespec time;
	time.tv_sec = -1;
	time.tv_nsec = 0;
	
	clock_gettime(CLOCK_MONOTONIC, &time);
	linuxClock->currentTime = getCurrentTime(&time);
	linuxClock->startTime += (linuxClock->currentTime - linuxClock->pausedTime);
	
	
	return clock;
}

double getCurrentTime(const struct timespec* time) {
  return ((double)(time->tv_sec) + (double)(time->tv_nsec * 0.000000001));
}
