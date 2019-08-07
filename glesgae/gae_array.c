#include "gae_array.h"
#include "gae_memory.h"

#include <string.h>

gae_array_t* gae_array_init(gae_array_t* array, unsigned int size)
{
	array->size = size;
	array->allocated = 0;
	array->used = 0;
	array->data = 0;
	
	return array;
}

gae_array_t* gae_array_resize(gae_array_t* array, unsigned int length)
{
	void* ptr = 0;
	unsigned int size = length * array->size;
	
	if (array->allocated >= size)
		return array;
	
	ptr = gae_realloc(array->data, size);
	if (0 != ptr) {
		array->data = ptr;
		array->allocated = size;
	}
	
	return array;
}

gae_array_t* gae_array_push(gae_array_t* array, void* const data)
{
	void* ptr = 0;
	const unsigned int current = array->allocated;
	
	while (array->used + array->size > array->allocated) {
		gae_array_resize(array, gae_max(1, gae_array_length(array) << 1));
		if (current == array->allocated) { /* out of memory */
			return array;
		}
	}
	
	ptr = array->data + array->used;
	
	memcpy(ptr, data, array->size);
	
	array->used += array->size;
	
	return array;
}

gae_array_t* gae_array_set(gae_array_t* array, unsigned int index, void* const data)
{
	void* ptr = 0;
	const unsigned int pos = index * array->size;
	
	if (pos >= array->used - array->size) return array;
	
	ptr = array->data + pos;
	
	memcpy(ptr, data, array->size);
	
	return array;
}

gae_array_t* gae_array_delete(gae_array_t* array, unsigned int index)
{
	void* ptr = 0;
	void* last = 0;
	const unsigned int pos = index * array->size;
	
	if (pos > array->used - array->size) return array;
	
	ptr = array->data + pos;
	last = gae_array_last(array);
	
	if (ptr != last) memcpy(ptr, last, array->size);
	
	array->used -= array->size;
	
	return array;
}

gae_array_t* gae_array_foreach(gae_array_t* array, gae_array_foreach_t foreach)
{
	unsigned int i;
	unsigned int count = gae_array_length(array);
	
	for (i = 0; i < count; ++i) {
		void* itr = array->data + (i * array->size);
		(*foreach)(itr);
	}
	
	return array;
}

void* gae_array_first(gae_array_t* array)
{
	if (0 == array->used) return 0;
	else return array->data;
}

void* gae_array_get(gae_array_t* array, unsigned int index)
{
	const unsigned int pos = index * array->size;
	
	if (pos > array->used - array->size) return 0;
	else return array->data + pos;
}

void* gae_array_last(gae_array_t* array)
{
	const unsigned int pos = array->used - array->size;
	
	if ((0 == array->used)
	|| (array->allocated < array->used - array->size)) return 0;
	else return array->data + pos;
}

void* gae_array_end(gae_array_t* array)
{
	return array->data + array->used;
}

unsigned int gae_array_length(gae_array_t* const array)
{
	return array->used / array->size;
}

gae_array_t* gae_array_destroy(gae_array_t* array)
{
	if (0 != array->data) gae_free(array->data);
	array->data = 0;
	array->size = 0;
	array->allocated = 0;
	array->used = 0;
	
	return array;
}
