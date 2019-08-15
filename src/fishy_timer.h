#ifndef _FISHY_TIMER_H_
#define _FISHY_TIMER_H_

#include "gae.h"
#include "fishy_structs.h"

fishy_timer_t* fishy_timer_init(fishy_timer_t* timer);
fishy_timer_t* fishy_timer_update(fishy_timer_t* timer);
fishy_timer_t* fishy_timer_draw(fishy_timer_t* timer, gae_point_2d_t pos);

#endif
