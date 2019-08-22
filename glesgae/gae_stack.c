#include "gae_stack.h"

gae_stack_t* gae_stack_init(gae_stack_t* stack, unsigned int elementSize)
{
	return gae_array_init(stack, elementSize);
}

gae_stack_t* gae_stack_destroy(gae_stack_t* stack)
{
	return gae_array_destroy(stack);
}

void* gae_stack_peek(gae_stack_t* stack)
{
	return gae_array_last(stack);
}

void* gae_stack_pop(gae_stack_t* stack)
{
	void* last = gae_array_last(stack);
	if (0 == stack->used) return 0;
	
	stack->used -= stack->size;
	if (stack->used > stack->allocated) /* unsigned wrap */
		stack->used = 0;
	
	return last;
}

gae_stack_t* gae_stack_push(gae_stack_t* stack, void* data)
{
	return gae_array_push(stack, data);
}

gae_stack_t* gae_stack_replace(gae_stack_t* stack, void* data)
{
	gae_stack_pop(stack);
	return gae_stack_push(stack, data);
}

unsigned int gae_stack_count(gae_stack_t* stack)
{
	return gae_array_length(stack);
}
