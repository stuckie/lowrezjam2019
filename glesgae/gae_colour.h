#ifndef _GAE_COLOUR_H_
#define _GAE_COLOUR_H_

#include "gae_types.h"

typedef struct gae_colour_rgba_s {
	gae_byte r, g, b, a;
} gae_colour_rgba;

typedef struct gae_colour_hsv_s {
	double h; /* 0 - 360 */
	double s; /* 0 - 1.0 */
	double v; /* 0 - 1.0 */
} gae_colour_hsv;

gae_colour_rgba gae_colour_hsv_to_rgb(gae_colour_hsv hsv);

gae_colour_hsv gae_colour_rgb_to_hsv(gae_colour_rgba rgb);

#endif
