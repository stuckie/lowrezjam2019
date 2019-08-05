#include "gae_sprite_sheet.h"

#include "gae_js.h"
#include "gae_rect.h"

/* Key (name) : Value (rect) */
typedef struct sprite_node_s {
	char* name;
	gae_rect_t srcRect;
} sprite_node_t;

gae_sprite_sheet_t* gae_sprite_sheet_init(gae_sprite_sheet_t* sheet, gae_json_document_t* jsDoc)
{
	gae_json_node_t* sprites = gae_json_node_find(jsDoc, jsDoc->root, "sprites");
	(void)(sprites);
	return sheet;
}
