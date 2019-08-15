#include "gae.h"

#include "fishy_structs.h"
#include "fishy_timer.h"

#include <stdlib.h>

typedef struct reel_s {
	gae_point_2d_t reelOrigin;
	gae_point_2d_t largeBit;
	gae_point_2d_t smallBit;
	
	gae_hashstring reelId;
	gae_hashstring largeBitId;
	gae_hashstring smallBitId;
	
	gae_rect_t reelRect;
	gae_rect_t drawRect;
} reel_t;

static reel_t* reel_init(reel_t* reel)
{
	reel->reelId = gae_hashstring_calculate("reel");
	reel->largeBitId = gae_hashstring_calculate("reelLargeBit");
	reel->smallBitId = gae_hashstring_calculate("reelSmallBit");
	
	reel->reelOrigin.x = 9;
	reel->reelOrigin.y = 54;
	
	reel->largeBit.x = 15.5;
	reel->largeBit.y = 56.5;
	
	reel->smallBit.x = 5.5;
	reel->smallBit.y = 49.5;
	
	reel->reelRect.x = 4;
	reel->reelRect.y = 48;
	
	return reel;
}

typedef struct bobber_s {
	gae_point_2d_t origin;
	gae_point_2d_t pos;
	gae_point_2d_t target;
	gae_rect_t rect;
	
	gae_hashstring bobberIds[3];
	unsigned int currentBobAnim;
	
	gae_timer_t animTimer;
} bobber_t;

bobber_t* bobber_init(bobber_t* bobber)
{
	bobber->bobberIds[0] = gae_hashstring_calculate("float");
	bobber->bobberIds[1] = gae_hashstring_calculate("float2");
	bobber->bobberIds[2] = gae_hashstring_calculate("float3");
	
	bobber->currentBobAnim = 0;
	
	bobber->target.x = bobber->pos.x = bobber->origin.x = bobber->rect.x = 28;
	bobber->target.y = bobber->pos.y = bobber->origin.y = bobber->rect.y = 33;
	
	gae_timer_init(&bobber->animTimer, gae_system.main_clock);
	bobber->animTimer.scale = -1;
	bobber->animTimer.currentTime = 1.0F;
	
	return bobber;
}

typedef struct depthMeter_s {
	gae_timer_t timer;
	
	gae_hashstring depthBarId;
	gae_hashstring depthMarkerId;
	gae_point_2d_t depthPoint;
	gae_rect_t barRect;
	
	float depth;
} depthMeter_t;

depthMeter_t* depthMeter_init(depthMeter_t* meter)
{
	meter->depthPoint.x = 0;
	meter->depthPoint.y = 0;
	
	meter->depth = 0;
	
	meter->barRect.x = 6;
	meter->barRect.y = 43;
	
	meter->depthBarId = gae_hashstring_calculate("depth-meter");
	meter->depthMarkerId = gae_hashstring_calculate("merker-right");
	
	return meter;
}

enum item_state
{	ITEM_WAITING
,	ITEM_BITING
,	ITEM_FIGHTING
,	ITEM_ESCAPED
,	ITEM_LANDED
};

typedef struct item_s {
	float distance;
	float depth;
	float strength;
	
	gae_hashstring fishMarkerId;
	
	gae_timer_t aiTimer;
	
	enum item_state state;
} item_t;

item_t* item_init(item_t* item)
{
	item->distance = gae_rand_float(100.0F);
	item->depth = rand() % 10;
	item->strength = gae_rand_float(5.0F);
	
	item->fishMarkerId = gae_hashstring_calculate("marker-left");
	
	item->state = ITEM_WAITING;
	
	gae_timer_init(&item->aiTimer, gae_system.main_clock);
	item->aiTimer.currentTime = 0.5F + gae_rand_float(1.0F);
	
	return item;
}

typedef struct fishy_reel_s {
	gae_timer_t timer;
	
	float tension;
	float angle;
	float distance;
	
	reel_t reel;
	bobber_t bobber;
	depthMeter_t depthMeter;
	item_t item;
} fishy_reel_t;

static int onStart(void* userData)
{
	fishy_reel_t* data = userData;
	
	gae_timer_init(&data->timer, gae_system.main_clock);
	
	reel_init(&data->reel);
	bobber_init(&data->bobber);
	depthMeter_init(&data->depthMeter);
	item_init(&data->item);
	
	data->tension = 0;
	data->angle = 0;
	data->distance = 100;
	
	return 0;
}

reel_t* reel_update(reel_t* reel, fishy_reel_t* data)
{
	gae_colour_hsv colour;
	gae_colour_rgba displayColour;
	float tensionPercent = data->tension / 100;
	gae_point_2d_t largePos = gae_point2d_rotate(&reel->reelOrigin, gae_deg2rad(data->angle), &reel->largeBit);
	gae_point_2d_t smallPos = gae_point2d_rotate(&reel->reelOrigin, gae_deg2rad(data->angle), &reel->smallBit);
	gae_point_2d_t linePos;
	
	linePos.x = 64;
	linePos.y = reel->reelOrigin.y;
	
	colour.h = 0.0F;
	colour.s = 1.0F;
	colour.v = tensionPercent;
	
	displayColour = gae_colour_hsv_to_rgb(colour);
	gae_graphics_context_set_draw_colour(gae_system.graphics.context, &displayColour);
	
	gae_graphics_context_draw_line(gae_system.graphics.context, &reel->reelOrigin, &linePos);
	gae_colour_rgba_set_white(displayColour);
	gae_graphics_context_set_draw_colour(gae_system.graphics.context, &displayColour);
	
	gae_sprite_sheet_draw(&GLOBAL.sprites, reel->reelId, &reel->reelRect);
	reel->drawRect.x = largePos.x; reel->drawRect.y = largePos.y;
	gae_sprite_sheet_draw(&GLOBAL.sprites, reel->largeBitId, &reel->drawRect);
	reel->drawRect.x = smallPos.x; reel->drawRect.y = smallPos.y;
	gae_sprite_sheet_draw(&GLOBAL.sprites, reel->smallBitId, &reel->drawRect);
	
	
	return reel;
}

item_t* item_update(item_t* item, fishy_reel_t* data)
{
	switch (item->state) {
		case ITEM_WAITING: {
		};
		break;
		case ITEM_BITING: {
			if (0 >= data->bobber.animTimer.currentTime) {
				data->bobber.currentBobAnim++;
				if (data->bobber.currentBobAnim >= 3)
					data->bobber.currentBobAnim = 0;
			}
		};
		break;
		case ITEM_FIGHTING: {
		};
		break;
		case ITEM_ESCAPED: {
		};
		break;
		case ITEM_LANDED: {
		};
		break;
	}
	
	return item;
}

bobber_t* bobber_update(bobber_t* bobber, fishy_reel_t* data)
{
	gae_point_2d_t linePos;
	gae_colour_rgba colour;
	
	bobber->pos.x = gae_lerp(bobber->pos.x, bobber->target.x, 0.1);
	bobber->pos.y = gae_lerp(bobber->pos.y, bobber->target.y, 0.1);
	
	bobber->rect.x = bobber->pos.x;
	bobber->rect.y = bobber->pos.y;
	
	gae_sprite_sheet_draw(&GLOBAL.sprites, bobber->bobberIds[bobber->currentBobAnim], &bobber->rect);
	if (0 >= bobber->animTimer.currentTime) {
		bobber->animTimer.currentTime = gae_rand_float(1.0F);
		
		bobber->target.x = bobber->origin.x + gae_rand_float(5.0F);
		bobber->target.y = bobber->origin.y + gae_rand_float(5.0F);
	}
	gae_timer_update(&bobber->animTimer, gae_system.main_clock);
	
	linePos.x = 0;
	linePos.y = 32;
	
	gae_colour_rgba_set_black(colour);
	gae_graphics_context_set_draw_colour(gae_system.graphics.context, &colour);
	gae_graphics_context_draw_line(gae_system.graphics.context, &bobber->pos, &linePos);
	
	(void)(data);
	
	return bobber;
}

depthMeter_t* depthMeter_update(depthMeter_t* depthMeter, fishy_reel_t* data)
{
	gae_sprite_sheet_draw(&GLOBAL.sprites, depthMeter->depthBarId, &depthMeter->barRect);
	
	(void)(data);
	
	return depthMeter;
}

static int onUpdate(void* userData)
{
	fishy_reel_t* data = userData;
	gae_colour_rgba colour;
	gae_point_2d_t time;
	
	gae_timer_update(&data->timer, gae_system.main_clock);
	
	if (data->tension > 0) data->tension -= data->timer.deltaTime * 30;
	else data->tension = 0;
	
	if (GLOBAL.pointer.isDown[0]) {
		data->tension += data->timer.deltaTime * 60;
		data->angle += data->timer.deltaTime * 180;
	}
	
	if (100 < data->tension) { /* SNAP! */
		return 1;
	}
	
	reel_update(&data->reel, data);
	item_update(&data->item, data);
	bobber_update(&data->bobber, data);
	depthMeter_update(&data->depthMeter, data);
	
	colour.r = 0; colour.g = 179; colour.b = 255; colour.a = 0;
	gae_graphics_context_set_draw_colour(gae_system.graphics.context, &colour);
	
	time.x = 0;
	time.y = 0;
	
	fishy_timer_draw(&GLOBAL.time, time);
	
	return 0;
}

static int onStop(void* userData)
{
	fishy_reel_t* data = userData;
	(void)(data);
	
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
