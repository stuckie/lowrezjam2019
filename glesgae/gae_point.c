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
