#include "gae.h"

#include <string.h>

gae_sprite_t* gae_sprite_init(gae_sprite_t* sprite, gae_graphics_texture_t* const texture, gae_rect_t* const src)
{
	sprite->frame.texture = texture;
	sprite->frame.src = *src;
	sprite->dst.x = 0; sprite->dst.y = 0;
	sprite->dst.w = src->w; sprite->dst.h = src->h;

	return sprite;
}

gae_sprite_t* gae_sprite_draw(gae_sprite_t* sprite)
{
	gae_graphics_context_blit_texture(gae_system.graphics.context, sprite->frame.texture, &(sprite->frame.src), &(sprite->dst));
	
	return sprite;
}

gae_sprite_t* gae_sprite_destroy(gae_sprite_t* sprite)
{
	return sprite;
}

gae_sprite_anim_t* gae_sprite_anim_init(gae_sprite_anim_t* sprite, gae_graphics_texture_t* const texture, gae_rect_t* const srcs, unsigned int count)
{
	unsigned int i;

	sprite->animTimer = 0;
	sprite->animSpeed = 1000;
	sprite->current = 0;
	sprite->dst.x = 0; sprite->dst.y = 0;
	sprite->dst.w = 64; sprite->dst.h = 64;

	gae_list_init(&sprite->frames);
	for (i = 0; i < count; ++i) {
		gae_frame_t* frame = gae_alloc(sizeof(gae_frame_t));
		gae_list_node_t* node = gae_alloc(sizeof(gae_list_node_t));
		
		gae_list_node_init(node, frame);
		
		frame->texture = texture;
		frame->src = srcs[i];

		gae_list_add(&sprite->frames, node);

		sprite->current = node;
	}

	return sprite;
}

gae_sprite_anim_t* gae_sprite_anim_update(gae_sprite_anim_t* sprite, unsigned int ticks)
{
	sprite->animTimer += ticks;

	if (sprite->animTimer >= sprite->animSpeed) {
		sprite->animTimer -= sprite->animSpeed;
		sprite->current = sprite->current->next;
		if (0 == sprite->current)
			sprite->current = sprite->frames.start;
	}
	
	return sprite;
}

gae_sprite_anim_t* gae_sprite_anim_draw(gae_sprite_anim_t* sprite)
{
	gae_frame_t* frame = sprite->current->data;
	gae_graphics_context_blit_texture(gae_system.graphics.context, frame->texture, &(frame->src), &(sprite->dst));
	
	return sprite;
}

static void gae_sprite_delete_frames(gae_list_node_t* node)
{
	gae_free(node->data);
	gae_free(node);
}

gae_sprite_anim_t* gae_sprite_anim_destroy(gae_sprite_anim_t* sprite)
{
	gae_list_foreach(&(sprite->frames), gae_sprite_delete_frames);
	
	return sprite;
}
