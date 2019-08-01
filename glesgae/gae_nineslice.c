#include "gae.h"

#include <stdio.h>

gae_nineslice_t* gae_nineslice_init(gae_nineslice_t* nineslice, unsigned int dimx[4], unsigned int dimy[4], gae_graphics_texture_t* const texture)
{
	unsigned int i;
	unsigned int pX = dimx[GAE_NINESLICE_U];
	unsigned int pL = dimx[GAE_NINESLICE_L];
	unsigned int pR = dimx[GAE_NINESLICE_R];
	unsigned int pW = dimx[GAE_NINESLICE_W];
	
	unsigned int wX = pL - pX;
	unsigned int wL = pR - wX;
	unsigned int wR = pW - pR - pX;
	
	unsigned int pY = dimy[GAE_NINESLICE_V];
	unsigned int pT = dimy[GAE_NINESLICE_T];
	unsigned int pB = dimy[GAE_NINESLICE_B];
	unsigned int pH = dimy[GAE_NINESLICE_H];
	
	unsigned int hY = pT - pY;
	unsigned int hT = pB - hY;
	unsigned int hB = pH - pB - pY;
	
	nineslice->texture = texture;
	
	for (i = 0; i < gae_nineslice_count; ++i) {
		gae_frame_t* frame = &(nineslice->frames[i]);
		gae_rect_t* rect = &(nineslice->dimensions[i]);
		
		frame->texture = nineslice->texture;
		switch (i) {
			case gae_nineslice_top_left: {
				frame->src.x = pX; frame->src.y = pY;
				frame->src.w = wX; frame->src.h = hY;
			};
			break;
			case gae_nineslice_top: {
				frame->src.x = pL; frame->src.y = pY;
				frame->src.w = wL; frame->src.h = hY;
			};
			break;
			case gae_nineslice_top_right: {
				frame->src.x = pR; frame->src.y = pY;
				frame->src.w = wR; frame->src.h = hY;
			};
			break;
			
			case gae_nineslice_centre_left: {
				frame->src.x = pX; frame->src.y = pT;
				frame->src.w = wX; frame->src.h = hT;
			};
			break;
			case gae_nineslice_centre: {
				frame->src.x = pL; frame->src.y = pT;
				frame->src.w = wL; frame->src.h = hT;
			};
			break;
			case gae_nineslice_centre_right: {
				frame->src.x = pR; frame->src.y = pT;
				frame->src.w = wR; frame->src.h = hT;
			};
			break;
			
			case gae_nineslice_bottom_left: {
				frame->src.x = pX; frame->src.y = pB;
				frame->src.w = wX; frame->src.h = hB;
			};
			break;
			case gae_nineslice_bottom: {
				frame->src.x = pL; frame->src.y = pB;
				frame->src.w = wL; frame->src.h = hB;
			};
			break;
			case gae_nineslice_bottom_right: {
				frame->src.x = pR; frame->src.y = pB;
				frame->src.w = wR; frame->src.h = hB;
			};
			break;
		};
		
		rect->x = frame->src.x; rect->y = frame->src.y;
		rect->w = frame->src.w; rect->h = frame->src.h;
	}
	
	nineslice->dst.x = 0; nineslice->dst.y = 0;
	nineslice->dst.w = wX + wL + wR; nineslice->dst.h = hY + hT + hB;
	
	return nineslice;
}

gae_nineslice_t* gae_nineslice_render(gae_nineslice_t* const nineslice)
{
	unsigned int i;
	
	for (i = 0; i < gae_nineslice_count; ++i) {
		gae_rect_t pos = nineslice->dst;
		pos.x += nineslice->dimensions[i].x;
		pos.y += nineslice->dimensions[i].y;
		pos.w = nineslice->dimensions[i].w;
		pos.h = nineslice->dimensions[i].h;
		gae_graphics_context_blit_texture(gae_system.graphics.context, nineslice->texture, &(nineslice->frames[i].src), &pos);
	}
	
	return nineslice;
}

gae_nineslice_t* gae_nineslice_update(gae_nineslice_t* nineslice)
{
	return nineslice;
}
