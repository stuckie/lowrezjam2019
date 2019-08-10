#include "gae_camera.h"

gae_camera_t* gae_camera_init(gae_camera_t* camera, gae_point_2d_t viewDims, gae_point_2d_t portDims)
{
	camera->view.x = 0;
	camera->view.y = 0;
	camera->view.w = viewDims.x;
	camera->view.h = viewDims.y;
	
	camera->port.x = 0;
	camera->port.y = 0;
	camera->port.w = portDims.x;
	camera->port.h = portDims.y;
	
	return camera;
}

gae_camera_t* gae_camera_move(gae_camera_t* camera, gae_point_2d_t delta)
{
	camera->view.x += delta.x;
	camera->view.y += delta.y;
	
	return camera;
}

gae_camera_t* gae_camera_position(gae_camera_t* camera, gae_point_2d_t absolute)
{
	camera->view.x = absolute.x;
	camera->view.y = absolute.y;
	
	return camera;
}

gae_camera_t* gae_camera_destroy(gae_camera_t* camera)
{
	return camera;
}
