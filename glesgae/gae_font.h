#ifndef _GAE_FONT_H_
#define _GAE_FONT_H_

#include "gae.h"

/* very quick and dirty font */
typedef struct gae_font_s
{
	gae_map_t lookup;
	gae_graphics_texture_t* texture;
	unsigned int glyphWidth;
	unsigned int glyphHeight;
} gae_font_t;

/* Create a font based on the given texture, and a string of definitions matching the layout of the font. IE: "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" */
gae_font_t* gae_font_init(gae_font_t* font, unsigned int width, unsigned int height, gae_graphics_texture_t* const texture, const char* definitions);

/* Set the colour of this font */
gae_font_t* gae_font_setColour(gae_font_t* font, gae_byte r, gae_byte g, gae_byte b);

/* Write a string based on this font directly to the screen at the given x, y coords */
gae_font_t* gae_font_writeText(gae_font_t* font, const char* string, const unsigned int x, const unsigned int y, const unsigned int spacing);

/* Destroy the given font */
gae_font_t* gae_font_destroy(gae_font_t* font);

#endif
