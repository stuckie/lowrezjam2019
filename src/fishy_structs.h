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

