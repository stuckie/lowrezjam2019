#include "gae.h"

typedef struct water_cell_s {
    int depth;
    int fish;
} water_cell_t;

typedef struct water_area_s {
    gae_grid_t area;
} water_area_t;

/* Create a new water area with the given width, height and list of fish */
water_area_t* water_area_init(water_area_t* area, unsigned int width, unsigned int height, gae_list_t* fish);

/* Print out the water area to the console */
water_area_t* water_area_print(water_area_t* area);

/* Clean up the given water area */
water_area_t* water_area_destroy(water_area_t* area);
