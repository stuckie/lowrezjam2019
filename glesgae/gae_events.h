#ifndef _GAE_EVENTS_H_
#define _GAE_EVENTS_H_

enum gae_event_type
{	GAE_EVENT_MOUSE_MOVE
,	GAE_EVENT_MOUSE_BUTTON
,	GAE_EVENT_KEYBOARD
,	GAE_EVENT_QUIT
,	GAE_EVENT_WINDOW_FULLSCREEN
};

typedef struct gae_event_s {
	enum gae_event_type type;
	void* event;
} gae_event_t;

typedef void (*gae_event_callback_t)(void* userDatum, gae_event_t* const event);
typedef struct gae_event_system_s {
	gae_event_callback_t onQuit;
	gae_event_callback_t onWindowEvent;
	gae_event_callback_t onMouseEvent;
	gae_event_callback_t onKeyboardEvent;
	gae_event_callback_t onJoystickEvent;
	gae_event_callback_t onControllerEvent;
	gae_event_callback_t onTouchEvent;
	void* userDatum;
} gae_event_system_t;

typedef struct gae_pointer_s {
	unsigned int x;
	unsigned int y;
	
	unsigned int isDown[5]; /* Left, Middle, Right, Scroll Up, Scroll Down */
} gae_pointer_t;

typedef struct gae_pointer_move_event_s {
	unsigned int x;
	unsigned int y;
} gae_pointer_move_event_t;

typedef struct gae_pointer_button_event_s {
	unsigned int x;
	unsigned int y;
	unsigned int buttonId;
	unsigned int isDown;
} gae_pointer_button_event_t;

typedef struct gae_keyboard_s {
	unsigned int down[512]; /* surely no-one has more than 512 keys... */
} gae_keyboard_t;

typedef struct gae_keyboard_event_s {
	unsigned int key;
	unsigned int isDown;
} gae_keyboard_event_t;

/* Initialise an event system with the given user data */
gae_event_system_t* gae_event_system_init(gae_event_system_t* system, void* userDatum);

/* Update the event system */
gae_event_system_t* gae_event_system_update(gae_event_system_t* const system);

/* Destroy an event system */
gae_event_system_t* gae_event_system_destroy(gae_event_system_t* system);

#endif

