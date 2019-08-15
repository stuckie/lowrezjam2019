#include "gae_point.h"

#include <math.h>

float gae_point_distancef(float x1, float y1, float x2, float y2)
{
	float dist = gae_point_distancef2(x1, y1, x2, y2);
	if (0 > dist) dist *= -1;
	return sqrt(dist);
}

float gae_point_distancef2(float x1, float y1, float x2, float y2)
{
	return ((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1));
}

int gae_point2d_in_rectangle(gae_point_2d_t point, gae_rect_t rect)
{
	return !(point.x < rect.x || point.y < rect.y || point.x > rect.x + rect.w || point.y > rect.y + rect.h);
}

float gae_point2d_angle_between(gae_point_2d_t* const a, gae_point_2d_t* const b)
{
	return atan2(a->y - b->y, a->x - b->x);
}

gae_point_2d_t gae_point2d_rotate(gae_point_2d_t* const origin, float radians, gae_point_2d_t* const p)
{
	gae_point_2d_t pp = *p;
	gae_point_2d_t rot;
	
	float s = sin(radians);
	float c = cos(radians);
	
	pp.x -= origin->x;
	pp.y -= origin->y;
	
	rot.x = pp.x * c - pp.y * s;
	rot.y = pp.x * s + pp.y * c;
	
	pp.x = rot.x + origin->x;
	pp.y = rot.y + origin->y;
	
	return pp;
}
