#ifndef _GAE_SYSTEM_H_
#define _GAE_SYSTEM_H_

struct gae_event_system_s;
struct gae_graphics_context_s;
struct gae_graphics_window_s;
struct gae_clock_s;

typedef struct gae_graphics_s {
	struct gae_graphics_window_s* window;
	struct gae_graphics_context_s* context;
} gae_graphics_t;

typedef struct gae_system_s {
	gae_graphics_t graphics;
	struct gae_event_system_s* event_system;
	struct gae_clock_s* main_clock;
} gae_system_t;

extern gae_system_t gae_system;

void gae_system_delay(unsigned int ms);

#endif
