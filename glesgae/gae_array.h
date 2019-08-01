#ifndef _GAE_ARRAY_H_
#define _GAE_ARRAY_H_

#include "gae_types.h"

typedef struct gae_array_s {
	gae_byte* data;				/* array data */
	
	unsigned int allocated;		/* how big data is */
	unsigned int used;			/* how much data is used */
	unsigned int size;			/* size of each element */
} gae_array_t;

typedef void (*gae_array_foreach_t)(void* data);

/* Initialise an array to hold elements of the given size */
gae_array_t* gae_array_init(gae_array_t* array, unsigned int size);

/* Resize the given array to length * size elements if possible */
gae_array_t* gae_array_resize(gae_array_t* array, unsigned int length);

/* Copy the given data into this array */
gae_array_t* gae_array_push(gae_array_t* array, void* const data);

/* Sets the nth element */
gae_array_t* gae_array_set(gae_array_t* array, unsigned int index, void* const data);

/* Deletes the nth element */
gae_array_t* gae_array_delete(gae_array_t* array, unsigned int index);

/* Foreach element, perform callback */
gae_array_t* gae_array_foreach(gae_array_t* array, gae_array_foreach_t foreach);

/* Get the first element */
void* gae_array_first(gae_array_t* array);

/* Get the nth element */
void* gae_array_get(gae_array_t* array, unsigned int index);

/* Get the last element */
void* gae_array_last(gae_array_t* array);

/* Get the end of the array - not the last element */
void* gae_array_end(gae_array_t* array);

/* Get the amount of elements in this array */
unsigned int gae_array_length(gae_array_t* const array);

/* Destroy the given array */
gae_array_t* gae_array_destroy(gae_array_t* array);

#endif
