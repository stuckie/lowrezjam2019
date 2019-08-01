#include "gae_memory.h"

#include <stdlib.h>

void* gae_alloc(unsigned int size)
{
	return malloc((size_t)size);
}

void* gae_realloc(void* ptr, unsigned int size)
{
	return realloc(ptr, (size_t)size);
}

void gae_free(void* ptr)
{
	free(ptr);
}
