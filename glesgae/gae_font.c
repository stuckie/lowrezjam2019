#include "gae.h"

#include <string.h>

/* Create a font based on the given texture, and a string of definitions matching the layout of the font. IE: "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" */
gae_font_t* gae_font_init(gae_font_t* font, unsigned int width, unsigned int height, gae_graphics_texture_t* const texture, const char* definitions)
{
	const unsigned long chars = strlen(definitions);
	int i = 0;
	gae_map_t* lookups = &(font->lookup);
	font->glyphWidth = width;
	font->glyphHeight = height;
	font->texture = texture;
	
	gae_map_init(lookups, sizeof(gae_hashstring), sizeof(gae_frame_t), gae_hashstring_compare);
	for (i = 0; i < chars; ++i) {
		char c = definitions[i];
		gae_hashstring id = (gae_hashstring)c;
		gae_frame_t frame;
		
		frame.texture = texture;
		frame.src.x = (i * width) % texture->w;
		frame.src.y = ((i * height) / texture->w) * width;
		frame.src.w = width;
		frame.src.h = height;
		
		gae_map_push(lookups, &id, &frame);
	}
	
	return font;
}

gae_font_t* gae_font_setColour(gae_font_t* font, gae_colour_rgba* const colour)
{
	gae_graphics_context_texture_colour(gae_system.graphics.context, font->texture, colour);
	return font;
}

/* Write a string based on this font directly to the screen at the given x, y coords */
gae_font_t* gae_font_writeText(gae_font_t* font, const char* string, const unsigned int x, const unsigned int y, const unsigned int spacing)
{
	const unsigned long chars = strlen(string);
	int i = 0;
	gae_rect_t dst;
	dst.x = x; dst.y = y; 
	dst.w = font->glyphWidth; dst.h = font->glyphHeight;
	for (i = 0; i < chars; ++i) {
		gae_hashstring id = (gae_hashstring)string[i];
		gae_frame_t* frame = (gae_frame_t*)gae_map_get(&(font->lookup), &id);	
		dst.x = x + (i * font->glyphWidth) + (i * spacing);
		if (0 != frame) gae_graphics_context_blit_texture(gae_system.graphics.context, frame->texture, &(frame->src), &dst);
	}
	
	return font;
}

/* Destroy the given font */
gae_font_t* gae_font_destroy(gae_font_t* font)
{
	gae_map_destroy((&font->lookup));
	
	return font;
}

