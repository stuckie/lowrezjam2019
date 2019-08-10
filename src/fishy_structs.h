#ifndef _FISHY_STRUCTS_H_
#define _FISHY_STRUCTS_H_

#include "gae.h"

typedef struct fish_framerate_s {
	gae_timer_t cap;
	float fps;
	float ticksPerFrame;
} fish_framerate_t;

enum fishy_controller_action_state
{	ACTION_UP
,	ACTION_DOWN
,	ACTION_PRESSED
,	ACTION_RELEASED
};

typedef struct fishy_controller_s {
	enum fishy_controller_action_state action;
	unsigned int x;
	unsigned int y;
} fishy_controller_t;

typedef struct fish_global_s {
	gae_sprite_sheet_t sprites;
	fish_framerate_t framerate;
	gae_stack_t stateStack;
	
	gae_pointer_t pointer;
	gae_keyboard_t keyboard;
} fish_global_t;

typedef struct fish_def_s {
	int min_depth;
	int max_depth;

	int min_weight;
	int max_weight;

	gae_list_t bait;
} fish_def_t;

typedef struct fish_s {
	char name[16]; 
	int depth;
	int weight;
	int bait;
} fish_t;

typedef struct rod_s {
	char name[16];
	int type;
	int cost;
	int reach;
} rod_t;

typedef struct weight_s {
	char name[16];
	int type;
	int cost;
	int depth;
} weight_t;

typedef struct bait_s {
	char name[16];
	int type;
	int cost;
} bait_t;


typedef struct line_s {
	char name[16];
	int type;
	int cost;
	int strength;
} line_t;

typedef struct boat_s {
	char name[16];
	int type;
	int cost;
	int fuel;
} boat_t;

typedef struct player_rod_s {
	boat_t boat;
	rod_t rod;
	weight_t weight;
	bait_t bait;
	line_t line;
} player_rod_t;

extern fish_global_t GLOBAL;

#endif
