#ifndef _GAE_BUFFER_H_
#define _GAE_BUFFER_H_

#include "gae_types.h"

struct gae_file_s;

enum gae_buffer_type
{	GAE_BUFFER_FIXED
/*,	GAE_BUFFER_RING*/
,	GAE_BUFFER_EXPANSIVE
};

typedef struct gae_buffer_s {
	gae_byte* data;
	
	enum gae_buffer_type type;
	unsigned long pos;
	unsigned long length;
} gae_buffer_t;

gae_buffer_t* gae_buffer_init(gae_buffer_t* buffer, enum gae_buffer_type type, unsigned int size);

gae_buffer_t* gae_buffer_from_file(gae_buffer_t* buffer, struct gae_file_s* file);

gae_buffer_t* gae_buffer_read(gae_buffer_t* buffer, unsigned int size, void* data);

gae_buffer_t* gae_buffer_write(gae_buffer_t* buffer, unsigned int size, void* data);

gae_buffer_t* gae_buffer_destroy(gae_buffer_t* buffer);

#endif
