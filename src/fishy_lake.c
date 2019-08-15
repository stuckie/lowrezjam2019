#include "fishy_structs.h"
#include "fishy_area.h"
#include "fishy_timer.h"

#include "gae.h"

#include <math.h>

enum boat_state
{	BOAT_IDLE
,	BOAT_MOVING
,	BOAT_STOPPING
};

typedef struct lake_boat_s {
	gae_point_2d_t pos;
	gae_point_2d_t target;
	gae_point_2d_t tile;
	
	gae_hashstring boatIds[2];
	int currentBoatAnim;
	
	gae_rect_t boatRect;
	gae_timer_t animTimer;
	
	enum boat_state state;
} lake_boat_t;

lake_boat_t* lake_boat_init(lake_boat_t* boat)
{
	boat->pos.x = 0; boat->pos.y = 0;
	boat->target.x = 0; boat->target.x = 0;
	boat->tile.x = 0; boat->tile.x = 0;
	
	boat->boatIds[0] = gae_hashstring_calculate("boat");
	boat->boatIds[1] = gae_hashstring_calculate("boat2");
	boat->currentBoatAnim = 0;
	
	boat->boatRect.x = boat->pos.x;
	boat->boatRect.y = boat->pos.y;
	
	gae_timer_init(&boat->animTimer, gae_system.main_clock);
	boat->animTimer.currentTime = 1.0F;
	boat->animTimer.scale = -1;
	
	boat->state = BOAT_IDLE;
	
	return boat;
}

typedef struct fishy_lake_state_s {
	gae_graphics_texture_t waterTex;
	gae_graphics_texture_t minimapTex;
	gae_graphics_texture_t ui;
	
	gae_rect_t uiRect;
	gae_rect_t drawRect;
	
	gae_camera_t camera;
	gae_camera_t minimap;
	
	water_area_t waterArea;
	
	lake_boat_t boat;
	
	gae_button_t cast;
	gae_button_t shop;
	
	int buttonDown;
	int quit;
	int init;
} fishy_lake_state_t;

lake_boat_t* lake_boat_update(lake_boat_t* boat, fishy_lake_state_t* data)
{
	gae_timer_update(&boat->animTimer, gae_system.main_clock);
	if (0 > boat->animTimer.currentTime) {
		boat->animTimer.currentTime = 1.0F;
		boat->currentBoatAnim = !boat->currentBoatAnim;
	}
	
	if (boat->pos.x > boat->target.x) boat->pos.x -= 0.1f;
	else if (boat->pos.x < boat->target.x) boat->pos.x += 0.1F;
	if (fabs(boat->pos.x - boat->target.x) < 0.5F) boat->pos.x = boat->target.x;
	
	if (boat->pos.y > boat->target.y) boat->pos.y -= 0.1F;
	else if (boat->pos.y < boat->target.y) boat->pos.y += 0.1F;
	if (fabs(boat->pos.y - boat->target.y) < 0.5F) boat->pos.y = boat->target.y;
	
	boat->boatRect.x = boat->pos.x;
	boat->boatRect.y = boat->pos.y;
	
	if (boat->pos.x == boat->target.x && boat->pos.y == boat->target.y) {
		if (BOAT_MOVING == boat->state) {
			boat->state = BOAT_IDLE;
			data->cast.state = gae_button_active;
		}
	} else {
		boat->state = BOAT_MOVING;
		data->cast.state = gae_button_inactive;
	}
	
	gae_sprite_sheet_draw(&GLOBAL.sprites, boat->boatIds[boat->currentBoatAnim], &boat->boatRect);
	
	return boat;
}

static void fillWaterTexture(water_area_t* area, gae_graphics_texture_t* waterTex, int reverse)
{
	gae_buffer_t texBuffer;
	int i = 0;
	int size = area->area.columns * area->area.rows;
	gae_colour_hsv hsv;
	gae_colour_rgba rgba;
	
	gae_buffer_init(&texBuffer, GAE_BUFFER_FIXED, size * 3U);
	
	for (i = 0; i < size; ++i) {
		gae_grid_cell_t* cell = area->area.cells + i;
		water_cell_t* tile = cell->data;
		
		hsv.h = 200;
		hsv.s = 0.0;
		if (1 == reverse) hsv.v = 1.0F - (float)(tile->depth / 10.0F);
		else hsv.v = (float)(tile->depth / 10.0F);
		
		rgba = gae_colour_hsv_to_rgb(hsv);
		
		texBuffer.data[(i * 3) + 0] = rgba.r;
		texBuffer.data[(i * 3) + 1] = rgba.g;
		texBuffer.data[(i * 3) + 2] = rgba.b;
	}
	
	gae_graphics_texture_fill_from_buffer(waterTex, gae_system.graphics.context, &texBuffer, area->area.columns, area->area.rows, 24);
	
	gae_buffer_destroy(&texBuffer);
}

static void setupCameras(fishy_lake_state_t* data)
{
	gae_point_2d_t mainView, mainPort, miniView, miniPort;
	mainView.x = 8; mainView.y = 6;		/* width/height of view into texture */
	mainPort.x = 64; mainPort.y = 48;	/* width/height of port on screen */
	
	miniView.x = 8; miniView.y = 6;	/* width/height of view into texture */
	miniPort.x = 16; miniPort.y = 12;	/* width/height of port on screen */
	
	gae_camera_init(&data->camera, mainView, mainPort);
	gae_camera_init(&data->minimap, miniView, miniPort);
	
	/* Minimap is only in the bottom right corner of the ui */
	data->minimap.port.x = 48; data->minimap.port.y = 50;
}

static void onCastButton(void* userData)
{
	gae_state_t castState;
	
	fishy_cast_init(&castState);
	if (0 != castState.onStart) (*castState.onStart)(castState.userData);
	gae_stack_push(&GLOBAL.stateStack, &castState);
	
	(void)(userData);
}

static void onShopButton(void* userData)
{
	fishy_lake_state_t* data = userData;
	data->quit = 1;
}

static void makeButton(gae_button_t* button, const char* jsonDef, int x, int y, gae_button_callback_t onReleased, void* userData)
{
	gae_point_2d_t pos;
	pos.x = x;
	pos.y = y;
	
	gae_button_init(button, jsonDef, &pos, userData);
	button->onReleased = onReleased;
}

static int onStart(void* userData)
{
	fishy_lake_state_t* data = userData;
	
	data->buttonDown = GLOBAL.pointer.isDown[0];
	
	if (data->init) return 0;
	
	water_area_init(&data->waterArea, 8, 6, 0);
	fillWaterTexture(&data->waterArea, &data->minimapTex, 0);
	water_area_destroy(&data->waterArea);
	
	gae_graphics_context_texture_load_from_file(gae_system.graphics.context, "data/lake_ui.bmp", &data->ui);
	
	setupCameras(data);
	
	data->uiRect.x = 0;
	data->uiRect.y = 48;
	data->uiRect.w = 64;
	data->uiRect.h = 16;
	
	lake_boat_init(&data->boat);
	
	makeButton(&data->cast, "data/cast-button.json", 34, 50, onCastButton, data);
	makeButton(&data->shop, "data/shop-button.json", 1, 50, onShopButton, data);
	
	data->quit = 0;
	data->init = 1;
	
	return 0;
}

static void draw_minimap_point(lake_boat_t* boat, gae_rect_t port)
{
	gae_colour_rgba colour;
	
	port.x += (boat->pos.x / 8) * 2;
	port.y += (boat->pos.y / 8) * 2;
	port.w = 2;
	port.h = 2;
	
	gae_colour_rgba_set_green(colour);
	gae_graphics_context_set_draw_colour(gae_system.graphics.context, &colour);
	gae_graphics_context_draw_filled_rect(gae_system.graphics.context, &port);
}

static int onUpdate(void* userData)
{
	fishy_lake_state_t* data = userData;
	gae_point_2d_t pointer;
	gae_point_2d_t time;
	gae_colour_rgba colour;
	gae_rect_t pointerRect;
	gae_rect_t fillRect;
	int buttonDown = (0 == data->buttonDown && GLOBAL.pointer.isDown[0]);
	
	if (1 == data->quit) return 1;

	pointer.x = GLOBAL.pointer.x;
	pointer.y = GLOBAL.pointer.y;
	
	pointerRect.x = floor(pointer.x / 8) * 8;
	pointerRect.y = floor(pointer.y / 8) * 8;
	
	time.x = 13;
	time.y = 51;
	
	fillRect.x = 0; fillRect.y = 0; fillRect.w = 64; fillRect.h = 48;
	
	gae_colour_rgba_set_cyan(colour);
	gae_graphics_context_set_draw_colour(gae_system.graphics.context, &colour);
	gae_graphics_context_draw_filled_rect(gae_system.graphics.context, &fillRect);
	lake_boat_update(&data->boat, data);
	gae_sprite_sheet_draw(&GLOBAL.sprites, gae_hashstring_calculate("marker"), &pointerRect);
	gae_graphics_context_blit_texture(gae_system.graphics.context, &data->ui, 0, &data->uiRect);
	gae_button_update(&data->shop, &pointer, GLOBAL.pointer.isDown[0]);
	gae_button_update(&data->cast, &pointer, GLOBAL.pointer.isDown[0]);
	gae_colour_rgba_set_blue(colour);
	gae_graphics_context_texture_colour(gae_system.graphics.context, &data->minimapTex, &colour);
	gae_graphics_context_blit_texture(gae_system.graphics.context, &data->minimapTex, &data->minimap.view, &data->minimap.port);
	draw_minimap_point(&data->boat, data->minimap.port);
	fishy_timer_draw(&GLOBAL.time, time);
	
	if (1 == buttonDown) {
		if ((8 > pointer.x / 8)
		&& (6 > pointer.y / 8)) {
			data->boat.target.x = pointerRect.x;
			data->boat.target.y = pointerRect.y;
		}
	}
	
	data->buttonDown = GLOBAL.pointer.isDown[0];
	
	return 0;
}

static int onStop(void* userData)
{
	fishy_lake_state_t* data = userData;
	
	gae_graphics_texture_destroy(&data->ui);
	
	return 0;
}

gae_state_t* fishy_lake_init(gae_state_t* state)
{
	fishy_lake_state_t* data = gae_alloc(sizeof(fishy_lake_state_t));
	data->init = 0;
	data->drawRect.x = 0; data->drawRect.y = 0; data->drawRect.w = 64; data->drawRect.h = 64;
	
	state->userData = data;
	state->onStart = onStart;
	state->onUpdate = onUpdate;
	state->onStop = onStop;
	
	return state;
}
