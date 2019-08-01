#ifndef _GAE_MEMORY_H_
#define _GAE_MEMORY_H_

/* Allocate a new chunk of memory */
void* gae_alloc(unsigned int size);

/* Reallocate a chunk of memory */
void* gae_realloc(void* ptr, unsigned int size);

/* Free a chunk of memory */
void gae_free(void* ptr);

#endif
