#ifndef _GAE_MANAGER_H_
#define _GAE_MANAGER_H_

#include "gae_hashstring.h"
#include "gae_map.h"

typedef struct gae_manager_s {
	gae_map_t items;
} gae_manager_t;

gae_manager_t* gae_manager_init(gae_manager_t* manager, unsigned int dataSize);

void* gae_manager_add(gae_manager_t* manager, gae_hashstring name);

void* gae_manager_get(gae_manager_t* manager, gae_hashstring name);

gae_manager_t* gae_manager_remove(gae_manager_t* manager, gae_hashstring name);

unsigned int gae_manager_count(gae_manager_t* manager);

gae_manager_t* gae_manager_destroy(gae_manager_t* manager);

#endif
