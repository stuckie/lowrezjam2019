#ifndef _GAE_TYPES_H_
#define _GAE_TYPES_H_

#define GAE_PI 3.14159265358979323846F

#define gae_invalid(x) (void)(x)
#define gae_deg2rad(x) (x * GAE_PI / 180.0F)
#define gae_rad2deg(x) (x * 180.0F / GAE_PI)

#define gae_min(a, b)	((a) < (b) ? (a) : (b))
#define gae_max(a, b)	((a) > (b) ? (a) : (b))

#define gae_lerp(a, b, time)			( (a) + (time) * (float) ((b) - (a)) )
#define gae_clamp(val, low, hi)			((val) < (low) ? (low) : (val) > (hi) ? (hi) : (val))

typedef unsigned char gae_byte;

typedef union gae_colour_rgba_s {
	gae_byte r, g, b, a;
	unsigned int value;
} gae_colour_rgba;

typedef struct gae_colour_hsv_s {
	float h; /* 0 - 360 */
	float s; /* 0 - 100 */
	float v; /* 0 - 100 */
} gae_colour_hsv;

#endif
