#include "gae_graphics_sdl2.h"
#include "gae_rect.h"
#include "gae_point.h"
#include "gae_buffer.h"
#include "gae_colour.h"

gae_graphics_window_t* gae_graphics_window_init(gae_graphics_window_t* window, const char* const name, int x, int y, int w, int h, unsigned int flags)
{
	window->data = SDL_CreateWindow(name, x, y, w, h, flags);
	window->name = name;
	window->x = x;
	window->y = y;
	window->w = w;
	window->h = h;
	
	return window;
}

gae_graphics_window_t* gae_graphics_window_destroy(gae_graphics_window_t* window)
{
	SDL_DestroyWindow(window->data);
	window->data = 0;
	window->name = 0;
	window->x = 0;
	window->y = 0;
	window->w = 0;
	window->h = 0;
	
	return window;
}

gae_graphics_context_t* gae_graphics_context_init(gae_graphics_context_t* context, gae_graphics_window_t* window, void* platformData)
{
	gae_graphics_context_sdl2_data_t* sdlData = platformData;
	if (0 == sdlData)
		context->data = SDL_CreateRenderer(window->data, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	else
		context->data = SDL_CreateRenderer(window->data, sdlData->index, sdlData->flags);

	return context;
}

gae_graphics_context_t* gae_graphics_context_clear(gae_graphics_context_t* const context)
{
	SDL_RenderClear(context->data);
    
	return context;
}

gae_graphics_context_t* gae_graphics_context_present(gae_graphics_context_t* const context)
{
	SDL_RenderPresent(context->data);
    
	return context;
}

gae_graphics_context_t* gae_graphics_context_texture_load_from_file(gae_graphics_context_t* const context, const char* const filepath, gae_graphics_texture_t* texture)
{
	SDL_Surface* surf = SDL_LoadBMP(filepath);
	texture->w = surf->w;
	texture->h = surf->h;
	texture->data = SDL_CreateTextureFromSurface(context->data, surf);
	SDL_FreeSurface(surf);

	return context;
}

gae_graphics_context_t* gae_graphics_context_blit_texture(gae_graphics_context_t* const context, gae_graphics_texture_t* const texture, gae_rect_t* const src, gae_rect_t* const dest)
{
	SDL_RenderCopy(context->data, texture->data, (SDL_Rect*)src, (SDL_Rect*)dest);
    
	return context;
}

gae_graphics_context_t* gae_graphics_context_blit_texture_params(gae_graphics_context_t* const context, gae_graphics_context_blit_params_t* const params)
{
	SDL_Point p;
	if (0 != params->origin) {
		p.x = params->origin->x;
		p.y = params->origin->y;
	} else { p.x = p.y = 0; }
	
	SDL_RenderCopyEx(context->data, params->texture->data, (SDL_Rect*)(params->srcRect), (SDL_Rect*)(params->dstRect), params->degrees, &p, SDL_FLIP_NONE);
	
	return context;
}

gae_graphics_context_t* gae_graphics_context_texture_colour(gae_graphics_context_t* const context, gae_graphics_texture_t* texture, gae_colour_rgba* const colour)
{
	SDL_SetTextureColorMod(texture->data, colour->r, colour->g, colour->b);
	
	return context;
}

gae_graphics_context_t* gae_graphics_context_set_draw_colour(gae_graphics_context_t* const context, gae_colour_rgba* const colour)
{
	SDL_SetRenderDrawColor(context->data, colour->r, colour->g, colour->b, colour->a);
	
	return context;
}

gae_graphics_context_t* gae_graphics_context_get_draw_colour(gae_graphics_context_t* const context, gae_colour_rgba* colour)
{
	SDL_GetRenderDrawColor(context->data, &colour->r, &colour->g, &colour->b, &colour->a);
	
	return context;
}

gae_graphics_context_t* gae_graphics_context_draw_point(gae_graphics_context_t* const context, gae_point_2d_t* const p)
{
	SDL_RenderDrawPoint(context->data, p->x, p->y);
	
	return context;
}

gae_graphics_context_t* gae_graphics_context_draw_line(gae_graphics_context_t* const context, gae_point_2d_t* const a, gae_point_2d_t* const b)
{
	SDL_RenderDrawLine(context->data, a->x, a->y, b->x, b->y);
	
	return context;
}

gae_graphics_context_t* gae_graphics_context_draw_rect(gae_graphics_context_t* const context, gae_rect_t* const rect)
{
	SDL_RenderDrawRect(context->data, (SDL_Rect*)rect);
	
	return context;
}

gae_graphics_context_t* gae_graphics_context_draw_filled_rect(gae_graphics_context_t* const context, gae_rect_t* const rect)
{
	SDL_RenderFillRect(context->data, (SDL_Rect*)rect);
	
	return context;
}

gae_graphics_context_t* gae_graphics_context_set_render_size(gae_graphics_context_t* const context, int w, int h)
{
	SDL_RenderSetLogicalSize(context->data, w, h);
	
	return context;
}

gae_graphics_context_t* gae_graphics_context_destroy(gae_graphics_context_t* context)
{
	SDL_DestroyRenderer(context->data);
	context->data = 0;
	
	return context;
}

gae_graphics_texture_t* gae_graphics_texture_init(gae_graphics_texture_t* texture)
{
	texture->data = 0;
	texture->w = 0;
	texture->h = 0;
	
	return texture;
}

gae_graphics_context_t* gae_graphics_context_init_render_target(gae_graphics_context_t* context, gae_graphics_texture_t* texture, int width, int height, int depth)
{
	texture->w = width;
	texture->h = height;
	
	if (32 == depth)
		texture->data = SDL_CreateTexture(context->data, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	else if (24 == depth)
		texture->data = SDL_CreateTexture(context->data, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, width, height);
	
	return context;
}

gae_graphics_context_t* gae_graphics_context_set_render_target(gae_graphics_context_t* context, gae_graphics_texture_t* const texture)
{
	if (0 != texture)
		SDL_SetRenderTarget(context->data, texture->data);
	else
		SDL_SetRenderTarget(context->data, 0);
	
	return context;
}

static gae_graphics_texture_t* gae_graphics_texture_from_surface(gae_graphics_texture_t* texture, SDL_Renderer* renderer, SDL_Surface* surface)
{
	Uint32 format;
	int access;
	
	gae_graphics_texture_init(texture);
	
	texture->data = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_QueryTexture(texture->data, &format, &access, (int*)&texture->w, (int*)&texture->h);
	
	return texture;
}

gae_graphics_texture_t* gae_graphics_texture_fill_from_buffer(gae_graphics_texture_t* texture, gae_graphics_context_t* const context, struct gae_buffer_s* const buffer, int width, int height, int depth)
{
	int pitch = (24 == depth) ? 3 * width : 4 * height;
	Uint32 rmask, gmask, bmask, amask;
	SDL_Surface* surf;
	
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif
	
	surf = SDL_CreateRGBSurfaceFrom((void*)buffer->data, width, height, depth, pitch, rmask, gmask, bmask, amask);
	
	gae_graphics_texture_from_surface(texture, context->data, surf);
	
	SDL_FreeSurface(surf);
	
	return texture;
}

gae_graphics_texture_t* gae_graphics_texture_destroy(gae_graphics_texture_t* texture)
{
	SDL_DestroyTexture(texture->data);
	texture->data = 0;
	texture->w = 0;
	texture->h = 0;
	
	return texture;
}
