#include "fishy_structs.h"

#include "gae.h"

#define SWING_SPEED 10

/* stupid idea
 * hold touch to swing rod.. let go to cast.. close to 45o we get, the better
 */

enum swing
{	swing_back
,	swing_fore
,	swing_cast
};

typedef struct fishy_cast_s {
	gae_timer_t timer;
	
	float backSwing;
	float foreSwing;
	
	float lockedBackSwing;
	float lockedForeSwing;
	
	gae_sprite_sheet_t sprites;
	gae_graphics_texture_t ui;
	gae_rect_t castRect;
	gae_rect_t backSwingPoint;
	gae_rect_t foreSwingPoint;
	
	int buttonDown;
	enum swing swing;
} fishy_cast_t;

static int onStart(void* userData)
{
	fishy_cast_t* data = userData;
	gae_json_document_t jsDoc;
	
	gae_json_document_init(&jsDoc, "data/sprites.json");
	gae_json_document_parse(&jsDoc);
	gae_sprite_sheet_init(&data->sprites, &jsDoc);
	gae_json_document_destroy(&jsDoc);
	
	gae_graphics_texture_init(&data->ui);
	gae_graphics_context_texture_load_from_file(gae_system.graphics.context, "data/cast_ui.bmp", &data->ui);
	
	gae_timer_init(&data->timer, gae_system.main_clock);
	data->timer.scale = SWING_SPEED;
	
	data->castRect.x = 34;
	data->castRect.y = 37;
	data->castRect.w = 29;
	data->castRect.h = 4;
	
	data->backSwingPoint.x = 48;
	data->backSwingPoint.y = 38;
	data->backSwingPoint.h = 1;
	data->backSwingPoint.w = -10;
	
	data->foreSwingPoint.x = 48;
	data->foreSwingPoint.y = 39;
	data->foreSwingPoint.h = 1;
	data->foreSwingPoint.w = 10;
	
	data->backSwing = data->lockedBackSwing = 0;
	data->foreSwing = data->lockedForeSwing = 0;
	
	data->buttonDown = GLOBAL.pointer.isDown[0];
	
	data->swing = swing_back;
	
	return 0;
}

static void drawBackSwing(fishy_cast_t* data)
{
	gae_colour_rgba colour;
	
	if (swing_back == data->swing)
		data->backSwing += data->timer.deltaTime;
	
	gae_colour_rgba_set_yellow(colour);
	gae_graphics_context_set_draw_colour(gae_system.graphics.context, &colour);
	data->backSwingPoint.w = -data->backSwing;
	gae_graphics_context_draw_filled_rect(gae_system.graphics.context, &data->backSwingPoint);
}

static void drawForeSwing(fishy_cast_t* data)
{
	gae_colour_rgba colour;
	
	if (swing_fore == data->swing)
		data->foreSwing += data->timer.deltaTime;
	
	gae_colour_rgba_set_green(colour);
	gae_graphics_context_set_draw_colour(gae_system.graphics.context, &colour);
	data->foreSwingPoint.w = data->foreSwing;
	gae_graphics_context_draw_filled_rect(gae_system.graphics.context, &data->foreSwingPoint);
}

static void lockSwing(fishy_cast_t* data, int buttonDown)
{
	if (1 == buttonDown) {
		switch (data->swing) {
			case swing_back: { 	/* lock backswing and start foreswing */
				data->lockedBackSwing = data->backSwing;
				data->foreSwingPoint.x = data->backSwingPoint.x + data->backSwingPoint.w;
				data->swing = swing_fore;
			};
			break;
			case swing_fore: {							/* cast the line! */
				data->lockedForeSwing = data->foreSwing;
				data->swing = swing_cast;
			};
			break;
			case swing_cast: {
				gae_colour_rgba colour;
				gae_state_t reelState;
				gae_state_t* current = gae_stack_peek(&GLOBAL.stateStack);
				if (0 != current->onStop) (*current->onStop)(data);
	
				fishy_reel_init(&reelState);
				if (0 != reelState.onStart) (*reelState.onStart)(reelState.userData);
				gae_stack_replace(&GLOBAL.stateStack, &reelState);
				
				colour.r = 0; colour.g = 180; colour.b = 255; colour.a = 255;
				gae_graphics_context_set_draw_colour(gae_system.graphics.context, &colour);
			};
			break;
		}
	}
}

static int onUpdate(void* userData)
{
	fishy_cast_t* data = userData;
	gae_colour_rgba colour;
	int buttonDown = 0;
	gae_point_2d_t start;
	gae_point_2d_t length;
	gae_point_2d_t end;
	
	if (0 == data->buttonDown && 1 == GLOBAL.pointer.isDown[0]) {
		data->buttonDown = 1;
		buttonDown = 1;
	}
	
	if (1 == data->buttonDown && 0 == GLOBAL.pointer.isDown[0])
		data->buttonDown = 0;
	
	gae_timer_update(&data->timer, gae_system.main_clock);
	
	gae_graphics_context_blit_texture(gae_system.graphics.context, &data->ui, 0, 0);
	
	gae_colour_rgba_set_white(colour);
	gae_graphics_context_set_draw_colour(gae_system.graphics.context, &colour);
	gae_graphics_context_draw_filled_rect(gae_system.graphics.context, &data->castRect);
	
	drawBackSwing(data);
	drawForeSwing(data);
	
	gae_sprite_sheet_draw(&data->sprites, gae_hashstring_calculate("cast-meter"), &data->castRect);
	
	if (data->backSwing >= 14) data->timer.scale = -SWING_SPEED;
	else if (data->backSwing <= 0) data->timer.scale = SWING_SPEED;
	if (data->castRect.x - data->foreSwingPoint.x + data->foreSwing > 27) data->timer.scale = -SWING_SPEED * 2;
	else if (data->foreSwing < 0) data->timer.scale = SWING_SPEED * 2;
	
	lockSwing(data, buttonDown);
	start.x = 21; start.y = 38;
	length.x = start.x;
	length.y = start.y - 10;
	
	switch (data->swing) {
		case swing_back: {
			end = gae_point2d_rotate(&start, gae_deg2rad(-data->backSwing * 6), &length);
			gae_graphics_context_draw_line(gae_system.graphics.context, &start, &end);
		};
		break;
		case swing_fore: {
			end = gae_point2d_rotate(&start, gae_deg2rad((-data->lockedBackSwing * 6) + data->foreSwing), &length);
			gae_graphics_context_draw_line(gae_system.graphics.context, &start, &end);
		};
		break;
		default:
		break;
	}
	
	return 0;
}

static int onStop(void* userData)
{
	fishy_cast_t* data = userData;
	
	gae_graphics_texture_destroy(&data->ui);
	gae_sprite_sheet_destroy(&data->sprites);
	
	return 0;
}

gae_state_t* fishy_cast_init(gae_state_t* state)
{
	fishy_cast_t* data = gae_alloc(sizeof(fishy_cast_t));
	
	state->userData = data;
	state->onStart = onStart;
	state->onUpdate = onUpdate;
	state->onStop = onStop;
	
	return state;
}
