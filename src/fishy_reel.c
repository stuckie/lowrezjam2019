#include "gae.h"

#include "fishy_structs.h"
#include "fishy_timer.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
	
	gae_hashstring bobberIds[4];
	unsigned int currentBobAnim;
	
	gae_hashstring waterIds[3];
	int currentWaterAnim;
	
	gae_timer_t animTimer;
	gae_timer_t waterTimer;

	int down;
} bobber_t;

bobber_t* bobber_init(bobber_t* bobber)
{
	bobber->bobberIds[0] = gae_hashstring_calculate("float");
	bobber->bobberIds[1] = gae_hashstring_calculate("float2");
	bobber->bobberIds[2] = gae_hashstring_calculate("float3");
	bobber->bobberIds[3] = gae_hashstring_calculate("float2");
	
	bobber->waterIds[0] = gae_hashstring_calculate("water-float1");
	bobber->waterIds[1] = gae_hashstring_calculate("water-float2");
	bobber->waterIds[2] = gae_hashstring_calculate("water-float3");
	
	bobber->currentBobAnim = 0;
	bobber->currentWaterAnim = -1;
	
	bobber->target.x = bobber->pos.x = bobber->origin.x = bobber->rect.x = 28;
	bobber->target.y = bobber->pos.y = bobber->origin.y = bobber->rect.y = 33;
	
	gae_timer_init(&bobber->animTimer, gae_system.main_clock);
	bobber->animTimer.scale = -1;
	bobber->animTimer.currentTime = 1.0F;
	
	gae_timer_init(&bobber->waterTimer, gae_system.main_clock);
	bobber->waterTimer.scale = -2;
	bobber->waterTimer.currentTime = 1.0F;

	bobber->down = GLOBAL.pointer.isDown[GAE_MOUSE_BUTTON_ANY];
	
	return bobber;
}

typedef struct depthMeter_s {
	gae_hashstring depthBarId;
	gae_hashstring depthMarkerId;
	
	gae_rect_t barRect;
} depthMeter_t;

depthMeter_t* depthMeter_init(depthMeter_t* meter)
{	
	meter->barRect.x = 54;
	meter->barRect.y = 7;
	
	meter->depthBarId = gae_hashstring_calculate("depth-meter");
	meter->depthMarkerId = gae_hashstring_calculate("marker-up");
	
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
	
	gae_hashstring leftId;
	gae_hashstring rightId;
	
	enum item_state state;
} item_t;

item_t* item_init(item_t* item)
{
	item->distance = 50 + gae_rand_float(50.0F);
	item->depth = 10 + rand() % 25;
	item->strength = rand() % 5;
	
	item->leftId = gae_hashstring_calculate("marker-left");
	item->rightId = gae_hashstring_calculate("marker-right");
	
	item->state = ITEM_WAITING;
	
	return item;
}

typedef struct fishy_reel_s {
	gae_timer_t timer;
	
	float tension;
	float angle;
	float distance;
	float depth;
	
	reel_t reel;
	bobber_t bobber;
	depthMeter_t depthMeter;
	item_t item;
	
	gae_hashstring fishGoneId;
	gae_hashstring lineBreakId;
	
	int showInfo;
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
	data->depth = 0;
	
	data->showInfo = 0;
	
	data->fishGoneId = gae_hashstring_calculate("fish-gone");
	data->lineBreakId = gae_hashstring_calculate("line-break");
	
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
	
	linePos.x = reel->reelOrigin.x + 7 + ((data->distance / 100) * 48);
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
	gae_rect_t rect;
	gae_colour_hsv hsv;
	gae_colour_rgba rgb;
	float distance = fabs((double)data->distance - item->distance);
	float depth = fabs((data->depth) - item->depth);
	
	rect.y = data->depthMeter.barRect.y + 1 + (item->depth);
	
	switch (item->state) {
		case ITEM_WAITING: {
			if (distance < 1) {
				gae_colour_rgba_set_red(rgb);
				if (depth < 1)
					item->state = ITEM_BITING;
			} else {
				hsv.h = 0;
				hsv.s = 0;
				hsv.v = 1.0F - (distance / 100.0F);
				
				rgb = gae_colour_hsv_to_rgb(hsv);
			}
		};
		break;
		case ITEM_BITING: {
			gae_colour_rgba_set_red(rgb);
			rect.y = data->depthMeter.barRect.y + 1 + (data->depth);
			if (0 >= data->bobber.animTimer.currentTime) {
				data->bobber.currentBobAnim++;
				if (data->bobber.currentBobAnim >= 4) {
					data->bobber.currentBobAnim = 0;
					item->depth = (10 + rand() % 25);
					item->distance = data->distance - (10 + rand() % 10);
					item->distance = gae_max(0, item->distance);
					item->state = ITEM_WAITING;
				}
				data->bobber.animTimer.currentTime = 1.0F;
			}
		};
		break;
		case ITEM_FIGHTING: {
			gae_colour_rgba_set_green(rgb);
			rect.y = data->depthMeter.barRect.y + 1 + (data->depth);
			if (0 >= data->bobber.animTimer.currentTime) {
				data->bobber.animTimer.currentTime = gae_rand_float(1.0F);
				
				data->bobber.target.x = data->bobber.origin.x + gae_rand_float(5.0F);
				data->bobber.target.y = data->bobber.origin.y + gae_rand_float(5.0F);
			}
		};
		break;
		case ITEM_ESCAPED: {
		};
		break;
		case ITEM_LANDED: {
		};
		break;
	}
	
	gae_graphics_context_texture_colour(gae_system.graphics.context, &GLOBAL.sprites.texture, &rgb);
	
	if (item->distance < data->distance) {
		rect.x = data->depthMeter.barRect.x - 4;
		gae_sprite_sheet_draw(&GLOBAL.sprites, item->rightId, &rect);
	} else {
		rect.x = data->depthMeter.barRect.x + data->depthMeter.barRect.w + 1 ;
		gae_sprite_sheet_draw(&GLOBAL.sprites, item->leftId, &rect);
	}
	
	gae_colour_rgba_set_white(rgb);
	gae_graphics_context_texture_colour(gae_system.graphics.context, &GLOBAL.sprites.texture, &rgb);
	
	item->distance -= 0.05;
	if (0 >= item->distance)
		item->state = ITEM_ESCAPED;
	
	return item;
}

bobber_t* bobber_update(bobber_t* bobber, fishy_reel_t* data)
{
	gae_point_2d_t linePos;
	gae_colour_rgba colour;
	
	bobber->pos.x = gae_lerp(bobber->pos.x, bobber->target.x, 0.1);
	bobber->pos.y = gae_lerp(bobber->pos.y, bobber->target.y + bobber->currentBobAnim, 0.1);
	
	bobber->rect.x = bobber->pos.x;
	bobber->rect.y = bobber->pos.y;
	
	gae_sprite_sheet_draw(&GLOBAL.sprites, bobber->bobberIds[bobber->currentBobAnim], &bobber->rect);
	
	linePos.x = 0;
	linePos.y = 32;
	
	gae_colour_rgba_set_black(colour);
	gae_graphics_context_set_draw_colour(gae_system.graphics.context, &colour);
	gae_graphics_context_draw_line(gae_system.graphics.context, &bobber->pos, &linePos);
	
	switch (data->item.state) {
		case ITEM_BITING: {
			gae_rect_t waterRect = bobber->rect;
			int hooked = rand() % 100;
			waterRect.x -= 4;
			waterRect.y += 4;
			
			gae_timer_update(&bobber->animTimer, gae_system.main_clock);
			gae_timer_update(&bobber->waterTimer, gae_system.main_clock);
			
			if (0 == data->bobber.down && GLOBAL.pointer.isDown[GAE_MOUSE_BUTTON_ANY]) {
				if (25 > hooked)
					data->item.state = ITEM_FIGHTING;
			}
			
			data->bobber.down = GLOBAL.pointer.isDown[GAE_MOUSE_BUTTON_ANY];

			if (0 >= bobber->waterTimer.currentTime) {
				bobber->waterTimer.currentTime = 1.0F;
				bobber->currentWaterAnim++;
				bobber->currentWaterAnim %= 3;
			}
			gae_sprite_sheet_draw(&GLOBAL.sprites, bobber->waterIds[bobber->currentWaterAnim], &waterRect);
		}
		break;
		case ITEM_FIGHTING: {
			gae_rect_t waterRect = bobber->rect;
			waterRect.x -= 4;
			waterRect.y += 4;
			gae_timer_update(&bobber->animTimer, gae_system.main_clock);
			gae_timer_update(&bobber->waterTimer, gae_system.main_clock);
			if (0 >= bobber->waterTimer.currentTime) {
				bobber->waterTimer.currentTime = 1.0F;
				bobber->currentWaterAnim++;
				bobber->currentWaterAnim %= 3;
			}
			gae_sprite_sheet_draw(&GLOBAL.sprites, bobber->waterIds[bobber->currentWaterAnim], &waterRect);
		}
		break;
		default:
		break;
	}
	
	return bobber;
}

depthMeter_t* depthMeter_update(depthMeter_t* depthMeter, fishy_reel_t* data)
{
	gae_rect_t rect;
	rect.x = depthMeter->barRect.x + 1;
	rect.y = depthMeter->barRect.y + 1 + (data->depth);
	
	gae_sprite_sheet_draw(&GLOBAL.sprites, depthMeter->depthBarId, &depthMeter->barRect);
	gae_sprite_sheet_draw(&GLOBAL.sprites, depthMeter->depthMarkerId, &rect);
	
	return depthMeter;
}

static int onUpdate(void* userData)
{
	fishy_reel_t* data = userData;
	gae_colour_rgba colour;
	gae_point_2d_t time;
	
	gae_timer_update(&data->timer, gae_system.main_clock);
	
	if (0 != data->showInfo) {
		switch (data->showInfo) {
			case 1: { /* line break */
				gae_rect_t rect;
				rect.x = 20; rect.y = 26; rect.w = 5; rect.h = 5;
				gae_sprite_sheet_draw(&GLOBAL.sprites, data->lineBreakId, &rect);
			};
			break;
			case 2: { /* fish gone */
				gae_rect_t rect;
				rect.x = 24; rect.y = 22;
				gae_sprite_sheet_draw(&GLOBAL.sprites, data->fishGoneId, &rect);
			};
			break;
			default:
			break;
		}

		if (0 >= data->timer.currentTime) {
			fishy_timer_paused(&GLOBAL.time);
			return 1;
		}

		return 0;
	}
	
	if (GLOBAL.pointer.isDown[GAE_MOUSE_BUTTON_ANY]) {
		data->tension += data->timer.deltaTime * (20 + (data->item.state == ITEM_FIGHTING ? data->item.strength : 0));
		data->angle += data->timer.deltaTime * 180;
		data->distance -= data->timer.deltaTime * 10;
		data->depth -= data->timer.deltaTime * 40;
	} else {
		data->depth += data->timer.deltaTime * 30;
		data->tension -= data->timer.deltaTime * (15 - (data->item.state == ITEM_FIGHTING ? data->item.strength : 0));
	}
	
	if (data->item.state == ITEM_ESCAPED) { /* It got away! */
		data->showInfo = 2;
		data->timer.currentTime = 3;
		data->timer.scale = -1;
		
		return 0;
	}
	
	if (100 < data->tension) { /* SNAP! */
		data->showInfo = 1;
		data->timer.currentTime = 3;
		data->timer.scale = -1;
		
		return 0;
	}
	
	if (0 > data->distance) { /* Landed? */
		if (data->item.state == ITEM_FIGHTING) {
			gae_state_t landed;
			gae_state_t* thisState;
			int i = rand() % 16;
			char c[12];
#if !defined(WINDOWS)
			sprintf(c, "%d", i);
#else
			sprintf_s(c, 12, "%d", i);
#endif
			GLOBAL.itemCatch = gae_hashstring_calculate(c);
			GLOBAL.trophies[i] = 1;
			GLOBAL.time.timer.currentTime += 60;
			
			fishy_land_init(&landed);
			if (0 != landed.onStart) (*landed.onStart)(landed.userData);
			thisState = gae_stack_peek(&GLOBAL.stateStack);
			if (0 != thisState->onStop) (*thisState->onStop)(thisState->userData);
			gae_stack_replace(&GLOBAL.stateStack, &landed);
			
			return 0;
		}
		return 1;
	}
	
	data->depth = gae_max(0, data->depth);
	data->depth = gae_min(data->depth, 38);
	data->tension = gae_max(0, data->tension);
	
	reel_update(&data->reel, data);
	bobber_update(&data->bobber, data);
	depthMeter_update(&data->depthMeter, data);
	item_update(&data->item, data);
	
	colour.r = 0; colour.g = 179; colour.b = 255; colour.a = 0;
	gae_graphics_context_set_draw_colour(gae_system.graphics.context, &colour);
	
	time.x = 0;
	time.y = 0;
	
	fishy_timer_update(&GLOBAL.time);
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
