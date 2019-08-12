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

/* pinched from stackoverflow.com/questions/13408990 */
#define gae_rand_float(__val) (float)rand()/(float)(RAND_MAX/__val)

typedef unsigned char gae_byte;

#endif
