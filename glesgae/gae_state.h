#ifndef _GAE_STATE_H_
#define _GAE_STATE_H_

typedef int (*gae_state_enter_t)(void* userData);
typedef int (*gae_state_update_t)(void* userData);
typedef int (*gae_state_exit_t)(void* userData);

typedef struct gae_state_s {
	gae_state_enter_t onStart;
	gae_state_update_t onUpdate;
	gae_state_exit_t onStop;
	
	void* userData;
} gae_state_t;

/* Initialise a new state object, with the given user data */
gae_state_t* gae_state_init(gae_state_t* state, void* userData);

/* Destroy the given state object */
gae_state_t* gae_state_destroy(gae_state_t* state);

#endif
