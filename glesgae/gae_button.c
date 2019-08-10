#include "gae.h"

static gae_hashstring* button_inactive = 0;
static gae_hashstring* button_active = 0;
static gae_hashstring* button_hover = 0;
static gae_hashstring* button_pressed = 0;

static void checkButtonIds()
{
	if (0 == button_inactive) {		
		button_inactive = gae_alloc(sizeof(gae_hashstring));
		button_active = gae_alloc(sizeof(gae_hashstring));
		button_hover = gae_alloc(sizeof(gae_hashstring));
		button_pressed = gae_alloc(sizeof(gae_hashstring));
		
		*button_active = gae_hashstring_calculate("active");
		*button_inactive = gae_hashstring_calculate("inactive");
		*button_hover = gae_hashstring_calculate("hover");
		*button_pressed = gae_hashstring_calculate("pressed");
	}
}

gae_button_t* gae_button_init(gae_button_t* button, const char* buttonDef, gae_point_2d_t* const position, void* userData)
{
	gae_json_document_t jsDoc;
	
	gae_json_document_init(&jsDoc, buttonDef);
	gae_json_document_parse(&jsDoc);
	gae_sprite_sheet_init(&button->frames, &jsDoc);
	gae_json_document_destroy(&jsDoc);
	
	button->onEnter = 0;
	button->onExit = 0;
	button->onPressed = 0;
	button->onReleased = 0;
	
	button->dims.x = position->x;
	button->dims.y = position->y;
	button->dims.w = 0;
	button->dims.h = 0;
	button->userData = userData;
	
	button->state = gae_button_active;
	
	checkButtonIds();
	
	return button;
}

gae_button_t* gae_button_update(gae_button_t* button, gae_point_2d_t* const cursor, int isClicked)
{
	enum gae_button_state lastState = button->state;
	
	if (gae_button_inactive != button->state) {
		if (0 != gae_point2d_in_rectangle(*cursor, button->dims))
			button->state = 0 == isClicked ? gae_button_hover : gae_button_pressed;
		else
			button->state = gae_button_active;
		
		if (gae_button_hover == button->state && gae_button_hover != lastState) {
			if (0 != button->onEnter) (*button->onEnter)(button->userData);
		} else if (gae_button_hover != button->state && gae_button_hover == lastState) {
			if (0 != button->onExit) (*button->onExit)(button->userData);
		} 
		
		if (gae_button_pressed == button->state && gae_button_pressed != lastState) {
			if (0 != button->onPressed) (*button->onPressed)(button->userData);
		} else if (gae_button_pressed != button->state && gae_button_pressed == lastState && 0 == isClicked) {
			if (0 != button->onReleased) (*button->onReleased)(button->userData);
		}		
	}
	
	switch (button->state) {
		case gae_button_active:
			gae_sprite_sheet_draw(&button->frames, *button_active, &button->dims);
		break;
		case gae_button_inactive:
			gae_sprite_sheet_draw(&button->frames, *button_inactive, &button->dims);
		break;
		case gae_button_hover:
			gae_sprite_sheet_draw(&button->frames, *button_hover, &button->dims);
		break;
		case gae_button_pressed:
			gae_sprite_sheet_draw(&button->frames, *button_pressed, &button->dims);
		break;
	}
	
	return button;
}

gae_button_t* gae_button_destroy(gae_button_t* button)
{
	gae_sprite_sheet_destroy(&button->frames);
	
	return button;
}
