#ifndef _GAE_CAMERA_H_
#define _GAE_CAMERA_H_

#include "gae_point.h"
#include "gae_rect.h"

typedef struct gae_camera_s {
	gae_rect_t view;
	gae_rect_t port;
} gae_camera_t;

gae_camera_t* gae_camera_init(gae_camera_t* camera, gae_point_2d_t viewDims, gae_point_2d_t portDims);
gae_camera_t* gae_camera_move(gae_camera_t* camera, gae_point_2d_t delta);
gae_camera_t* gae_camera_position(gae_camera_t* camera, gae_point_2d_t absolute);
gae_camera_t* gae_camera_destroy(gae_camera_t* camera);

#endif
