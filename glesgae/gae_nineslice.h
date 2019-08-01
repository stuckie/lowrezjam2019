#ifndef _GAE_NINESLICE_H_
#define _GAE_NINESLICE_H_

#include "gae.h"

enum gae_nineslice_frame
{	gae_nineslice_top_left = 0
,	gae_nineslice_top
,	gae_nineslice_top_right
,	gae_nineslice_centre_left
,	gae_nineslice_centre
,	gae_nineslice_centre_right
,	gae_nineslice_bottom_left
,	gae_nineslice_bottom
,	gae_nineslice_bottom_right
,	gae_nineslice_count
};

#define GAE_NINESLICE_V 0
#define GAE_NINESLICE_T 1
#define GAE_NINESLICE_B 2
#define GAE_NINESLICE_H 3

#define GAE_NINESLICE_U 0
#define GAE_NINESLICE_L 1
#define GAE_NINESLICE_R 2
#define GAE_NINESLICE_W 3

typedef struct gae_nineslice_s {
	gae_graphics_texture_t* texture;
	gae_frame_t frames[gae_nineslice_count];
	gae_rect_t dimensions[gae_nineslice_count];
	gae_rect_t dst;
} gae_nineslice_t;

gae_nineslice_t* gae_nineslice_init(gae_nineslice_t* nineslice, unsigned int dimx[4], unsigned int dimy[4], struct gae_graphics_texture_s* const texture);

gae_nineslice_t* gae_nineslice_update(gae_nineslice_t* nineslice);

gae_nineslice_t* gae_nineslice_render(gae_nineslice_t* nineslice);

gae_nineslice_t* gae_nineslice_resize(gae_nineslice_t* nineslice, struct gae_rect_s* dim);

gae_nineslice_t* gae_nineslice_destroy(gae_nineslice_t* nineslice);

#endif
