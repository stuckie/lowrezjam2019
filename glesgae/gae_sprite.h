#ifndef _GAE_SPRITE_H_
#define _GAE_SPRITE_H_

#include "gae.h"
#include "gae_rect.h"

typedef struct gae_frame_s {
	gae_graphics_texture_t* texture;
	gae_rect_t src;
} gae_frame_t;

typedef struct gae_sprite_s {
	gae_frame_t frame;
	gae_rect_t dst;
} gae_sprite_t;

typedef struct gae_sprite_anim_s {
	gae_list_t frames;
	gae_rect_t dst;
	
	gae_list_node_t* current;
	unsigned int animTimer;
	unsigned int animSpeed;
} gae_sprite_anim_t;

/* Initialise a single-framed sprite off the given texture and using the chunk specified by the source rect */
gae_sprite_t* gae_sprite_init(gae_sprite_t* sprite, gae_graphics_texture_t* const texture, gae_rect_t* const src);

/* Draw the given sprite */
gae_sprite_t* gae_sprite_draw(gae_sprite_t* sprite);

/* Destroy the given sprite */
gae_sprite_t* gae_sprite_destroy(gae_sprite_t* sprite);

/* Initialise an animated sprite from the given sprite shet, using the frame rects specified, and the amount given */
gae_sprite_anim_t* gae_sprite_anim_init(gae_sprite_anim_t* sprite, gae_graphics_texture_t* const texture, gae_rect_t* const srcs, unsigned int count);

/* Update the given animated sprite */
gae_sprite_anim_t* gae_sprite_anim_update(gae_sprite_anim_t* sprite, unsigned int ticks);

/* Draw the given animated sprite */
gae_sprite_anim_t* gae_sprite_anim_draw(gae_sprite_anim_t* sprite);

/* Destroy this animated sprite */
gae_sprite_anim_t* gae_sprite_anim_destroy(gae_sprite_anim_t* sprite);

#endif
