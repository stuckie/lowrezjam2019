#ifndef _GAE_POINT_H_
#define _GAE_POINT_H_

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
/* point 3d api */
/* point 4d api */

#endif
