#ifndef _GAE_FILE_H_
#define _GAE_FILE_H_

typedef enum gae_file_status_e
{	GAE_FILE_NO_ERROR = 0
,	GAE_FILE_NOT_FOUND
,	GAE_FILE_READ_ERROR
,	GAE_FILE_WRITE_ERROR
,	GAE_FILE_SEEK_ERROR
,	GAE_FILE_END_OF_FILE
,	GAE_FILE_CLOSED
,	GAE_FILE_NO_FILE
} gae_file_status;

enum gae_file_mode_e
{	GAE_FILE_MODE_READ
,	GAE_FILE_MODE_WRITE
,	GAE_FILE_MODE_APPEND
};

enum gae_file_type_e
{	GAE_FILE_TYPE_ASCII
,	GAE_FILE_TYPE_BINARY
};

enum gae_file_seek_e
{	GAE_FILE_SEEK_ABSOLUTE
,	GAE_FILE_SEEK_RELATIVE
};

#define GAE_FILE_SEEK_START 0
#define GAE_FILE_SEEK_END -1

typedef struct gae_file_s {
	enum gae_file_mode_e mode;
	enum gae_file_type_e type;
	enum gae_file_status_e status;
	
	void* platform;
} gae_file_t;

gae_file_t* gae_file_init(gae_file_t* file, enum gae_file_mode_e mode, enum gae_file_type_e type);

gae_file_t* gae_file_open(gae_file_t* file, const char* path);

gae_file_t* gae_file_read(gae_file_t* file, unsigned long size, void* data);

gae_file_t* gae_file_write(gae_file_t* file, unsigned long size, void* data);

gae_file_t* gae_file_seek(gae_file_t* file, unsigned long position, enum gae_file_seek_e seek);

unsigned long gae_file_position(gae_file_t* file);

unsigned long gae_file_length(gae_file_t* file);

gae_file_t* gae_file_close(gae_file_t* file);

gae_file_t* gae_file_destroy(gae_file_t* file);

#endif
