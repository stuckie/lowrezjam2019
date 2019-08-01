#include "gae_manager.h"


gae_manager_t* gae_manager_init(gae_manager_t* manager, unsigned int dataSize)
{
	gae_map_init(&(manager->items), sizeof(gae_hashstring), dataSize, gae_hashstring_compare);
	
	return manager;
}

void* gae_manager_add(gae_manager_t* manager, gae_hashstring name)
{
	unsigned int index = gae_array_length(&(manager->items.ids)) - 1;
	
	if (manager->items.values.used == manager->items.values.allocated) {
		gae_array_resize(&(manager->items.values), gae_array_length(&(manager->items.values)) + 1);
		index += 1;
	}
	
	gae_array_push(&(manager->items.ids), &name);
	manager->items.values.used += manager->items.values.size;
	return gae_array_get(&(manager->items.values), index);
}

void* gae_manager_get(gae_manager_t* manager, gae_hashstring name)
{
	return gae_map_get(&(manager->items), &name);
}

gae_manager_t* gae_manager_delete(gae_manager_t* manager, gae_hashstring name)
{
	gae_map_remove(&(manager->items), &name);
	return manager;
}

unsigned int gae_manager_count(gae_manager_t* manager)
{
	return gae_map_length(&(manager->items));
}

gae_manager_t* gae_manager_destroy(gae_manager_t* manager)
{
	gae_map_destroy(&(manager->items));
	return manager;
}
