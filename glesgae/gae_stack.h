#ifndef _GAE_STACK_H_
#define _GAE_STACK_H_

#include "gae_array.h"

typedef gae_array_t gae_stack_t;

/* Initialise a new stack with the given element size */
gae_stack_t* gae_stack_init(gae_stack_t* stack, unsigned int elementSize);

/* Peek at the top element of this stack */
void* gae_stack_peek(gae_stack_t* stack);

/* Pops the current State */
void* gae_stack_pop(gae_stack_t* stack);

/* Pushes a new element onto the Stack */
gae_stack_t* gae_stack_push(gae_stack_t* stack, void* state);

/* Replaces the current top element with the given element */
gae_stack_t* gae_stack_replace(gae_stack_t* stack, void* state);

/* Returns the number of elements */
unsigned int gae_stack_count(gae_stack_t* stack);

/* Destroys the Stack */
gae_stack_t* gae_stack_destroy(gae_stack_t* stack);

#endif
