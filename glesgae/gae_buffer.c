#include "gae_types.h"
#include "gae_memory.h"
#include "gae_buffer.h"
#include "gae_file.h"

#include <string.h>

gae_buffer_t* gae_buffer_init(gae_buffer_t* buffer, enum gae_buffer_type type, unsigned int size)
{
	buffer->data = 0;
	buffer->length = size;
	buffer->pos = 0;
	buffer->type = type;
	
	if (0 != size)
		buffer->data = gae_alloc(sizeof(gae_byte) * size);
	
	return buffer;
}

gae_buffer_t* gae_buffer_from_file(gae_buffer_t* buffer, gae_file_t* file)
{
	unsigned long oldFilePos = gae_file_position(file);
	
	buffer->type = GAE_BUFFER_FIXED;
	buffer->length = gae_file_length(file);
	buffer->data = gae_alloc(sizeof(gae_byte) * (unsigned int)buffer->length + 1);
	buffer->pos = 0;
	
	gae_file_seek(file, GAE_FILE_SEEK_START, GAE_FILE_SEEK_ABSOLUTE);
	gae_file_read(file, buffer->length, (void*)buffer->data);
	gae_file_seek(file, oldFilePos, GAE_FILE_SEEK_ABSOLUTE);
	
	buffer->data[buffer->length] = '\0';
	
	return buffer;
}

gae_buffer_t* gae_buffer_read(gae_buffer_t* buffer, unsigned int size, void* data)
{
	unsigned int amount = size;
	if (buffer->length < buffer->pos + amount) {
		switch (buffer->type) {
			case GAE_BUFFER_EXPANSIVE:
			case GAE_BUFFER_FIXED: {
				/* Can only go up to the end of the buffer */
				amount -= ((buffer->pos + amount) - buffer->length);
				
				memcpy(data, buffer->data + buffer->pos, amount);
				
				buffer->pos += amount;
			};
			break;
#if 0
			case GAE_BUFFER_RING: {
				/* Pass one - read up to the end */
				unsigned int overflow = (buffer->pos + amount) - buffer->length;
				unsigned int offset = amount - overflow;
				amount = offset;
				
				memcpy(data, buffer->data + buffer->pos, amount);
				
				/* Pass two - read from beginning */
				memcpy(data + offset, buffer->data, overflow);
				
				buffer->pos = overflow;
			};
			break;
#endif
		};
	} else { /* just copy the next chunk */
		memcpy(data, buffer->data + buffer->pos, amount);
		
		buffer->pos += amount;
	}
	
	return buffer;
}

gae_buffer_t* gae_buffer_write(gae_buffer_t* buffer, unsigned int size, void* data)
{
	unsigned int amount = size;
	if (buffer->length < buffer->pos + amount) {
		switch (buffer->type) {
			case GAE_BUFFER_FIXED: {
				/* Can only go up to the end of the buffer */
				amount -= ((buffer->pos + amount) - buffer->length);
				
				memcpy(buffer->data + buffer->pos, data, amount);
				
				buffer->pos += amount;
			};
			break;
#if 0
			case GAE_BUFFER_RING: {
				/* Pass one - write up to the end */
				unsigned int overflow = (buffer->pos + amount) - buffer->length;
				unsigned int offset = amount - overflow;
				amount = offset;
				
				memcpy(buffer->data + buffer->pos, data, amount);
				
				/* Pass two - write from beginning */
				memcpy(buffer->data, data + offset, overflow);
				
				buffer->pos = overflow;
			};
			break;
#endif
			case GAE_BUFFER_EXPANSIVE: {
				/* Expand the buffer */
				unsigned long newSize = buffer->length + size;
				
				void* newData = gae_realloc(buffer->data, newSize);
				if (0 != newData) {
					buffer->data = newData;
					buffer->length = newSize;
					
					memcpy(buffer->data + buffer->pos, data, amount);
					buffer->pos += amount;
				}
			}
		};
	} else { /* just copy the next chunk */
		memcpy(buffer->data + buffer->pos, data, amount);
		
		buffer->pos += amount;
	}
	
	return buffer;
}

gae_buffer_t* gae_buffer_destroy(gae_buffer_t* buffer)
{
	if (0 != buffer->data)
		gae_free(buffer->data);
		
	buffer->data = 0;
	buffer->length = 0;
	buffer->pos = 0;
	
	return buffer;
}
