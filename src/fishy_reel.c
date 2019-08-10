#include "gae.h"

#include "fishy_structs.h"

typedef struct fishy_reel_s {
	gae_timer_t timer;
	
	float tension;
	
	gae_sprite_sheet_t sprites;
	gae_rect_t reelRect;
	gae_rect_t floatRect;
	
	int buttonDown;
} fishy_reel_t;

static int onStart(void* userData)
{
	fishy_reel_t* data = userData;
	gae_json_document_t jsDoc;
	
	gae_json_document_init(&jsDoc, "data/sprites.json");
	gae_json_document_parse(&jsDoc);
	gae_sprite_sheet_init(&data->sprites, &jsDoc);
	gae_json_document_destroy(&jsDoc);
	
	gae_timer_init(&data->timer, gae_system.main_clock);
	
	data->reelRect.x = 1;
	data->reelRect.y = 45;
	
	data->floatRect.x = 28;
	data->floatRect.y = 33;
	
	data->tension = 0;
	
	data->buttonDown = GLOBAL.pointer.isDown[0];
	
	return 0;
}

static int onUpdate(void* userData)
{
	fishy_reel_t* data = userData;
	int buttonDown = 0;
	
	if (0 == data->buttonDown && 1 == GLOBAL.pointer.isDown[0]) {
		data->buttonDown = 1;
		buttonDown = 1;
	}
	
	if (1 == data->buttonDown && 0 == GLOBAL.pointer.isDown[0])
		data->buttonDown = 0;
	
	gae_timer_update(&data->timer, gae_system.main_clock);
	
	if (buttonDown) data->tension++;
	
	gae_sprite_sheet_draw(&data->sprites, gae_hashstring_calculate("reel"), &data->reelRect);
	gae_sprite_sheet_draw(&data->sprites, gae_hashstring_calculate("float"), &data->floatRect);
	
	return 0;
}

static int onStop(void* userData)
{
	fishy_reel_t* data = userData;
	
	gae_sprite_sheet_destroy(&data->sprites);
	
	return 0;
}

gae_state_t* fishy_reel_init(gae_state_t* state)
{
	fishy_reel_t* data = gae_alloc(sizeof(fishy_reel_t));
	
	state->userData = data;
	state->onStart = onStart;
	state->onUpdate = onUpdate;
	state->onStop = onStop;
	
	return state;
}
