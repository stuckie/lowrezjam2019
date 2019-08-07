#ifndef _FISHY_MENU_MAIN_H_
#define _FISHY_MENU_MAIN_H_

struct gae_button_s;

typedef void(*gae_button_onEnter)(struct gae_button_s* button, void* data);
typedef void(*gae_button_onExit(struct gae_button_s* button, void* data);
typedef void(*gae_button_onPressed)(struct gae_button_s* button, void* data);
typedef void(*gae_button_onReleased)(struct gae_button_s* button, void* data);

typedef struct gae_button_s {
	struct gae_sprite_sheet_s* texture;
	gae_rect_t dst;
	
	gae_button_onEnter onEnter;
	gae_button_onExit onExit;
	gae_button_onPressed onPressed;
	gae_button_onReleased onReleased;
}

typedef struct fishy_menu_main_s {
	
} fishy_menu_main_t;

#endif
