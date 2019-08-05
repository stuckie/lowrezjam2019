#ifndef _GAE_GRAPHICS_H_
#define _GAE_GRAPHICS_H_

#include "gae_types.h"

typedef struct gae_graphics_window_s {
	void* data;
	
	const char* name;
	int x, y, w, h;
} gae_graphics_window_t;

typedef struct gae_graphics_context_s {
	void* data;
} gae_graphics_context_t;

typedef struct gae_graphics_texture_s {
	void* data;
	
	int w, h;
} gae_graphics_texture_t;

enum gae_graphics_window_flags
{	GAE_WINDOW_FULLSCREEN
,	GAE_WINDOW_WINDOWED
};

struct gae_rect_s;
struct gae_buffer_s;

/* Initialise a new window with the given name at the specified position, with the given width and height, and flags */
gae_graphics_window_t* gae_graphics_window_init(gae_graphics_window_t* window, const char* const name, int x, int y, int w, int h, unsigned int flags);

/* Destroy the given window */
gae_graphics_window_t* gae_graphics_window_destroy(gae_graphics_window_t* window);

/* Initialise a new context bound to the given window, with the relevant platform data */
gae_graphics_context_t* gae_graphics_context_init(gae_graphics_context_t* context, gae_graphics_window_t* window, void* platformData);

/* Clear the given context */
gae_graphics_context_t* gae_graphics_context_clear(gae_graphics_context_t* const context);

/* Present the given context */
gae_graphics_context_t* gae_graphics_context_present(gae_graphics_context_t* const context);

/* Load a texture into the given context fro the specified file */
gae_graphics_context_t* gae_graphics_context_texture_load_from_file(gae_graphics_context_t* const context, const char* const filepath, gae_graphics_texture_t* texture);

/* Blit the given texture to the context using the specified source and destination rects */
gae_graphics_context_t* gae_graphics_context_blit_texture(gae_graphics_context_t* const context, gae_graphics_texture_t* const texture, struct gae_rect_s* const src, struct gae_rect_s* const dest);

/* Modulate texture colour */
gae_graphics_context_t* gae_graphics_context_texture_colour(gae_graphics_context_t* const context, gae_graphics_texture_t* texture, gae_byte r, gae_byte g, gae_byte b);

/* Set context render size */
gae_graphics_context_t* gae_graphics_context_set_render_size(gae_graphics_context_t* const context, int w, int h);

/* Destroy the given context */
gae_graphics_context_t* gae_graphics_context_destroy(gae_graphics_context_t* context);

/* Initialise a new texture object */
gae_graphics_texture_t* gae_graphics_texture_init(gae_graphics_texture_t* texture);

/* Fill texture with the given buffer */
gae_graphics_texture_t* gae_graphics_texture_fill_from_buffer(gae_graphics_texture_t* texture, gae_graphics_context_t* const context, struct gae_buffer_s* const buffer, int width, int height, int depth);

/* Destroy the given texture */
gae_graphics_texture_t* gae_graphics_texture_destroy(gae_graphics_texture_t* texture);

#endif
