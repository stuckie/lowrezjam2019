#include "gae_state.h"

gae_state_t* gae_state_init(gae_state_t* state, void* userData)
{
	state->userData = userData;
	
	return state;
}

gae_state_t* gae_state_destroy(gae_state_t* state)
{
	return state;
}
