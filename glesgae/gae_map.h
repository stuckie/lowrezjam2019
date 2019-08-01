#ifndef _GAE_MAP_H_
#define _GAE_MAP_H_

#include "gae_array.h"

typedef int (*gae_map_compare_t)(void* const keyA, void* const keyB);

typedef struct gae_map_s {
	gae_array_t ids;				/* ids */
	gae_array_t values;				/* values */
	gae_map_compare_t compare;		/* id compare callback */
} gae_map_t;

/* Initialise a new Map to store elements of the given size. */
gae_map_t* gae_map_init(gae_map_t* map, unsigned int keySize, unsigned int dataSize, gae_map_compare_t compare);

/* Creates a contiguous chunk of memory for the specified amount of Array elements. */
gae_map_t* gae_map_reserve(gae_map_t* map, unsigned int amount);

/* Copies the data into the Map - data can be freed after this call. */
gae_map_t* gae_map_push(gae_map_t* map, void* const id, void* const data);

/* Returns an indexed value of the Map. This element should NOT be freed after use. */
void* gae_map_get(gae_map_t* map, void* const id);

/* Returns a pointer to the first element of the values array. */
void* gae_map_begin(gae_map_t* map);

/* Returns a pointer to the first element of the ids array. */
void* gae_map_ids(gae_map_t* map);

/* Removes an element from the Map.*/
gae_map_t* gae_map_remove(gae_map_t* map, void* const id);

/* Returns the length of this Map in amount of elements with 0 being empty. */
unsigned int gae_map_length(gae_map_t* map);

/* Deletes the Map and all memory it allocated. Any stray pointers will therefore be undefined. */
gae_map_t* gae_map_destroy(gae_map_t* map);

#endif
