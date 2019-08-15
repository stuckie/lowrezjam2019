#ifndef _GAE_POINT_H_
#define _GAE_POINT_H_

#include "gae_rect.h"

typedef struct gae_point_2d_s {
	float x;
	float y;
} gae_point_2d_t;

typedef struct gae_point_3d_s {
	float x;
	float y;
	float z;
} gae_point_3d_t;

typedef struct gae_point_4d_s {
	float x;
	float y;
	float z;
	float w;
} gae_point_4d_t;

/* float api */

/* work out float distance between two points, given as separate floats */
float gae_point_distancef(float x1, float y1, float x2, float y2);

/* work out float distance between two points, given as separate floats, leaving it squared */
float gae_point_distancef2(float x1, float y1, float x2, float y2);

/* point 2d api */

/* work out whether the given point is in the given rectangle. 1 - yes, 0 - no */
int gae_point2d_in_rectangle(gae_point_2d_t point, gae_rect_t rect);

/* returns new point rotated around an origin */
gae_point_2d_t gae_point2d_rotate(gae_point_2d_t* const origin, float radians, gae_point_2d_t* const p);

/* returns angle between two points */
float gae_point2d_angle_between(gae_point_2d_t* const a, gae_point_2d_t* const b);

/* point 3d api */
/* point 4d api */

#endif
