#include "gae_graphics_sdl2.h"
#include "gae_rect.h"


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

gae_graphics_context_t* gae_graphics_context_texture_colour(gae_graphics_context_t* const context, gae_graphics_texture_t* texture, gae_byte r, gae_byte g, gae_byte b)
{
	SDL_SetTextureColorMod(texture->data, r, g, b);
	
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

gae_graphics_texture_t* gae_graphics_texture_destroy(gae_graphics_texture_t* texture)
{
	SDL_DestroyTexture(texture->data);
	texture->data = 0;
	texture->w = 0;
	texture->h = 0;
	
	return texture;
}