#include "gae_sprite_sheet.h"

#include "gae_hashstring.h"
#include "gae_map.h"
#include "gae_graphics.h"
#include "gae_js.h"
#include "gae_rect.h"
#include "gae_system.h"
#include "gae_memory.h"

#include <stdlib.h>

/* Key (name) : Value (rect) */
typedef struct sprite_node_s {
	gae_hashstring name;
	gae_rect_t srcRect;
} sprite_node_t;

static void loadTexture(gae_graphics_texture_t* texture, gae_json_document_t* jsDoc, gae_json_node_t* node)
{
	char* value = gae_json_string(jsDoc, &node->value);
	gae_graphics_context_texture_load_from_file(gae_system.graphics.context, value, texture);
	gae_free(value);
}

static sprite_node_t parseSprite(gae_json_document_t* jsDoc, gae_json_node_t* node)
{
	sprite_node_t sprite;
	char* key = gae_json_string(jsDoc, &node->name);
	char* x = gae_json_string(jsDoc, &node->child->value);	/* yum */
	char* y = gae_json_string(jsDoc, &node->child->sibling->value); /* yum yum */
	char* w = gae_json_string(jsDoc, &node->child->sibling->sibling->value); /* would you like fries with that? */
	char* h = gae_json_string(jsDoc, &node->child->sibling->sibling->sibling->value); /* and cheeeese! */

	sprite.name = gae_hashstring_calculate(key);
	sprite.srcRect.x = atoi(x);
	sprite.srcRect.y = atoi(y);
	sprite.srcRect.w = atoi(w);
	sprite.srcRect.h = atoi(h);
	
	gae_free(key);
	gae_free(x);
	gae_free(y);
	gae_free(w);
	gae_free(h);

	return sprite;
}

gae_sprite_sheet_t* gae_sprite_sheet_init(gae_sprite_sheet_t* sheet, gae_json_document_t* jsDoc)
{
	gae_json_node_t* filename = gae_json_node_find(jsDoc, jsDoc->root, "filename");
	gae_json_node_t* sprites = gae_json_node_find(jsDoc, jsDoc->root, "sprites");
	gae_json_node_t* sprite = sprites->child;
	
	gae_map_init(&sheet->data, sizeof(gae_hashstring), sizeof(gae_rect_t), gae_hashstring_compare);
	gae_graphics_texture_init(&sheet->texture);
	if (0 != filename) loadTexture(&sheet->texture, jsDoc, filename);
	
	while (0 != sprite) { /* sprites is an array of objects of arrays.. sprite = obj, sprite->child = inner array */
		sprite_node_t newSprite = parseSprite(jsDoc, sprite->child);
		gae_map_push(&sheet->data, &newSprite.name, &newSprite.srcRect);
		sprite = sprite->sibling;
	}
	
	return sheet;
}

gae_rect_t* gae_sprite_sheet_get_rect(gae_sprite_sheet_t* const sheet, gae_hashstring sprite)
{
	return gae_map_get(&sheet->data, &sprite);
}

gae_sprite_sheet_t* gae_sprite_sheet_draw(gae_sprite_sheet_t* const sheet, gae_hashstring sprite, gae_rect_t* const pos)
{
	gae_rect_t* drawRect = gae_map_get(&sheet->data, &sprite);
	if (0 == drawRect) return sheet;

	pos->w = drawRect->w;
	pos->h = drawRect->h;
	gae_graphics_context_blit_texture(gae_system.graphics.context, &sheet->texture, drawRect, pos);
	
	return sheet;
}

gae_sprite_sheet_t* gae_sprite_sheet_destroy(gae_sprite_sheet_t* sheet)
{
	gae_map_destroy(&sheet->data);
	gae_graphics_texture_destroy(&sheet->texture);
	
	return sheet;
}
