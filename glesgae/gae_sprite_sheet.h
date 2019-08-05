#ifndef _GAE_SPRITE_SHEET_H_
#define _GAE_SPRITE_SHEET_H_

#include "gae_graphics.h"
#include "gae_manager.h"

struct gae_json_document_s;

typedef struct gae_sprite_sheet_s {
	gae_graphics_texture_t texture;
	gae_manager_t data;
} gae_sprite_sheet_t;

gae_sprite_sheet_t* gae_sprite_sheet_init(gae_sprite_sheet_t* sheet, struct gae_json_document_s* jsDoc);

/*
 * Now, do we want to gae_sprite_sheet_draw("name", dstRect)?
 * Or do we want gae_sprite_sheet_rect("name", &srcRect) and draw later?
 */

gae_sprite_sheet_t* gae_sprite_sheet_destroy(gae_sprite_sheet_t* sheet);

#endif
