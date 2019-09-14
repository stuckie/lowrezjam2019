#ifndef _GAE_MEMORY_H_
#define _GAE_MEMORY_H_

/* Allocate a new chunk of memory */
void* gae_alloc(unsigned long size);

/* Reallocate a chunk of memory */
void* gae_realloc(void* ptr, unsigned long size);

/* Free a chunk of memory */
void gae_free(void* ptr);

#endif
