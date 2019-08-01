#include "gae.h"

#include <string.h>

gae_map_t* gae_map_init(gae_map_t* map, unsigned int keySize, unsigned int dataSize, gae_map_compare_t compare)
{
	gae_array_init(&(map->ids), keySize);
	gae_array_init(&(map->values), dataSize);
	map->compare = compare;

	return map;
}

gae_map_t* gae_map_reserve(gae_map_t* map, unsigned int amount)
{
	gae_array_resize(&(map->ids), amount);
	gae_array_resize(&(map->values), amount);

	return map;
}

gae_map_t* gae_map_push(gae_map_t* map, void* const id, void* const data)
{
	gae_array_push(&(map->ids), id);
	gae_array_push(&(map->values), data);

	return map;
}

void* gae_map_get(gae_map_t* map, void* const id) 
{
	void* found = 0;
	unsigned int index = 0U;
	const unsigned int size = gae_array_length(&(map->ids));

	while (index < size) {
		found = gae_array_get(&(map->ids), index);
		if (1 == map->compare(id, found))
			return gae_array_get(&(map->values), index);
		++index;
	}

	return 0;
}

void* gae_map_begin(gae_map_t* map)
{
	if (0 < gae_array_length(&(map->values)))
		return gae_array_get(&(map->values), 0);
	else
		return 0;
}

void* gae_map_ids(gae_map_t* map)
{
	if (0 < gae_array_length(&(map->ids)))
		return gae_array_get(&(map->ids), 0);
	else
		return 0;
}

gae_map_t* gae_map_remove(gae_map_t* map, void* const id)
{
	void* found = 0;
	unsigned int index = 0U;
	const unsigned int size = gae_array_length(&(map->ids));
	void* current = 0;
	void* temp = 0;

	while (index < size) {
		found = gae_array_get(&(map->ids), index);
		if (1 == map->compare(id, found)) { 
			/* copy last element into hole and pop off */
			temp = gae_array_get(&(map->values), size - 1U);
			current = gae_array_get(&(map->values), index);
			memcpy(current, temp, map->values.size);
			gae_array_delete(&(map->values), index);

			/* same with ids */
			temp = gae_array_get(&(map->ids), size - 1U);
			memcpy(found, temp, map->ids.size);
			gae_array_delete(&(map->ids), index);

			return map;
		}
		++index;
	}

	return map;
}

unsigned int gae_map_length(gae_map_t* map)
{
	return gae_array_length(&(map->ids));
}

gae_map_t* gae_map_destroy(gae_map_t* map) 
{
	gae_array_destroy(&(map->ids));
	gae_array_destroy(&(map->values));
	
	return map;
}

