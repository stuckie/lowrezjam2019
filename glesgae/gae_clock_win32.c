#include "gae_clock.h"
#include "gae_memory.h"

#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif

#include <stdlib.h>
#include <time.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

typedef struct gae_clock_win32_s {
	double startTime;
	double currentTime;
	double currentDelta;
	double pausedTime;
} gae_clock_win32_t;

#define MS_PER_SEC      1000ULL     // MS = milliseconds
#define US_PER_MS       1000ULL     // US = microseconds
#define HNS_PER_US      10ULL       // HNS = hundred-nanoseconds (e.g., 1 hns = 100 ns)
#define NS_PER_US       1000ULL

#define HNS_PER_SEC     (MS_PER_SEC * US_PER_MS * HNS_PER_US)
#define NS_PER_HNS      (100ULL)    // NS = nanoseconds
#define NS_PER_SEC      (MS_PER_SEC * US_PER_MS * NS_PER_US)

int clock_gettime_monotonic(struct timespec* tv)
{
	static LARGE_INTEGER ticksPerSec;
	LARGE_INTEGER ticks;
	double seconds;

	if (!ticksPerSec.QuadPart) {
		QueryPerformanceFrequency(&ticksPerSec);
		if (!ticksPerSec.QuadPart) {
			errno = ENOTSUP;
			return -1;
		}
	}

	QueryPerformanceCounter(&ticks);

	seconds = (double)ticks.QuadPart / (double)ticksPerSec.QuadPart;
	tv->tv_sec = (time_t)seconds;
	tv->tv_nsec = (long)((ULONGLONG)(seconds * NS_PER_SEC) % NS_PER_SEC);

	return 0;
}

double getCurrentTime(const struct timespec* time);

gae_clock_t* gae_clock_init(gae_clock_t* clock)
{
	gae_clock_win32_t* windowsClock = gae_alloc(sizeof(gae_clock_win32_t));
	
	struct timespec time;
	time.tv_sec = -1;
	time.tv_nsec = 0;
	
	clock->deltaTime = 0.0F;
	clock->currentTime = 0.0F;
	clock->platformData = windowsClock;
	
	clock_gettime_monotonic(&time);
	windowsClock->startTime = getCurrentTime(&time);
	windowsClock->currentTime = windowsClock->startTime;
	windowsClock->currentDelta = 0.0;
	windowsClock->pausedTime = 0.0;
	
	return clock;
}

gae_clock_t* gae_clock_destroy(gae_clock_t* clock) {
	gae_free(clock->platformData);
	clock->platformData = 0;

	return clock;
}

gae_clock_t* gae_clock_reset(gae_clock_t* clock) {
	gae_clock_win32_t* windowsClock = clock->platformData;
	
	struct timespec time;
	time.tv_sec = -1;
	time.tv_nsec = 0;
	
	clock_gettime_monotonic(&time);
	windowsClock->startTime = getCurrentTime(&time);
	windowsClock->currentTime = windowsClock->startTime;
	
	return clock;
}

gae_clock_t* gae_clock_update(gae_clock_t* clock) {
	gae_clock_win32_t* windowsClock = clock->platformData;
	
	struct timespec time;
	time.tv_sec = -1;
	time.tv_nsec = 0;
	
	clock_gettime_monotonic(&time);
	windowsClock->currentTime = getCurrentTime(&time);
	windowsClock->currentDelta = windowsClock->currentTime - windowsClock->startTime;
	
	clock->deltaTime = (float)windowsClock->currentDelta;
	clock->currentTime = (float)windowsClock->currentTime;
	
	return clock;
}

gae_clock_t* gae_clock_pause(gae_clock_t* clock) {
	gae_clock_win32_t* windowsClock = clock->platformData;
	
	struct timespec time;
	time.tv_sec = -1;
	time.tv_nsec = 0;
	
	clock_gettime_monotonic(&time);
	windowsClock->pausedTime = getCurrentTime(&time);
	
	return clock;
}

gae_clock_t* gae_clock_resume(gae_clock_t* clock) {
	gae_clock_win32_t* windowsClock = clock->platformData;
	
	struct timespec time;
	time.tv_sec = -1;
	time.tv_nsec = 0;
	
	clock_gettime_monotonic(&time);
	windowsClock->currentTime = getCurrentTime(&time);
	windowsClock->startTime += (windowsClock->currentTime - windowsClock->pausedTime);
	
	return clock;
}

double getCurrentTime(const struct timespec* time) {
  return ((double)(time->tv_sec) + (double)(time->tv_nsec * 0.000000001));
}
