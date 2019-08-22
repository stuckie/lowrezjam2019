#ifndef _FISHY_AREA_H_
#define _FISHY_AREA_H_

#include "gae.h"
#include "fishy_structs.h"

/* Create a new water area with the given width, height and list of fish */
water_area_t* water_area_init(water_area_t* area, unsigned int width, unsigned int height, gae_list_t* fish);

/* Print out the water area to the console */
water_area_t* water_area_print(water_area_t* area);

/* Clean up the given water area */
water_area_t* water_area_destroy(water_area_t* area);

#endif
