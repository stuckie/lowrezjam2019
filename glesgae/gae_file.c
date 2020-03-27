#include "gae_file.h"
#include "gae_memory.h"

#include <stdio.h>

typedef struct gae_file_stdio_s {
	FILE* file;
	
	unsigned long pos;
	unsigned long length;
} gae_file_stdio_t;

gae_file_t* gae_file_init(gae_file_t* file, enum gae_file_mode_e mode, enum gae_file_type_e type)
{
	gae_file_stdio_t* platform = gae_alloc(sizeof(gae_file_stdio_t));
	file->mode = mode;
	file->type = type;
	file->status = GAE_FILE_NO_FILE;
	
	file->platform = platform;
	platform->file = 0;
	platform->pos = 0;
	platform->length = 0;
	
	return file;
}

gae_file_t* gae_file_open(gae_file_t* file, const char* path)
{
	char options[2];
	gae_file_stdio_t* platform = file->platform;
	
	if ((GAE_FILE_NO_FILE != file->status)
	&& (GAE_FILE_CLOSED != file->status))
		return file;
	
	switch (file->mode) {
		case GAE_FILE_MODE_READ: options[0] = 'r'; break;
		case GAE_FILE_MODE_WRITE: options[0] = 'w'; break;
		case GAE_FILE_MODE_APPEND: options[0] = 'a'; break;
		default: break;
	}
	
	switch (file->type) {
		case GAE_FILE_TYPE_ASCII: options[1] = '\0'; break;
		case GAE_FILE_TYPE_BINARY: options[1] = 'b'; break;
		default: break;
	}

#if !defined(WINDOWS)
	platform->file = fopen(path, options);
#else
	fopen_s(&platform->file, path, options);
#endif

	if (0 == platform->file) {
		file->status = GAE_FILE_NOT_FOUND;
		return file;
	} else {
		file->status = GAE_FILE_NO_ERROR;
		fseek(platform->file, 0, SEEK_END);
		platform->length = ftell(platform->file);
		rewind(platform->file);
	}
	
	return file;
}

gae_file_t* gae_file_read(gae_file_t* file, unsigned long size, void* data)
{
	gae_file_stdio_t* platform = file->platform;
	/* TODO: control how many bytes are read.. speed up I/O a bit */
	unsigned long readAmount = fread(data, 1, size, platform->file);
	if (readAmount < size) {
		if (0 != feof(platform->file))
			file->status = GAE_FILE_END_OF_FILE;
		else if (0 != ferror(platform->file))
			file->status = GAE_FILE_READ_ERROR;
	} else
		file->status = GAE_FILE_NO_ERROR;
	
	platform->pos += readAmount;
	return file;
}

gae_file_t* gae_file_write(gae_file_t* file, unsigned long size, void* data)
{
	gae_file_stdio_t* platform = file->platform;
	/* TODO: control how many bytes are written.. speed up I/O a bit */
	unsigned long writeAmount = fwrite(data, 1, size, platform->file);
	if (writeAmount < size)
		file->status = GAE_FILE_WRITE_ERROR;
	else
		file->status = GAE_FILE_NO_ERROR;
	
	platform->pos += writeAmount;
	return file;
}

gae_file_t* gae_file_seek(gae_file_t* file, unsigned long position, enum gae_file_seek_e seek)
{
	gae_file_stdio_t* platform = file->platform;
	if (position == (unsigned int)GAE_FILE_SEEK_END) {
		fseek(platform->file, 0, SEEK_END);
		platform->pos = platform->length;
		return file;
	}
	
	switch (seek) {
		case GAE_FILE_SEEK_ABSOLUTE: {
			if (0 != fseek(platform->file, position, SEEK_SET))
				file->status = GAE_FILE_SEEK_ERROR;
			else
				platform->pos = position;
		};
		break;
		case GAE_FILE_SEEK_RELATIVE: {
			if (0 != fseek(platform->file, position, SEEK_CUR))
				file->status = GAE_FILE_SEEK_ERROR;
			else
				platform->pos += position;
		};
	};
	
	return file;
}

unsigned long gae_file_position(gae_file_t* file)
{
	gae_file_stdio_t* platform = file->platform;
	return platform->pos;
}

unsigned long gae_file_length(gae_file_t* file)
{
	gae_file_stdio_t* platform = file->platform;
	return platform->length;
}

gae_file_t* gae_file_close(gae_file_t* file)
{
	gae_file_stdio_t* platform = file->platform;
	if (GAE_FILE_NO_ERROR != file->status) return file;
	
	fclose(platform->file);
	file->status = GAE_FILE_CLOSED;
	
	return file;
}

gae_file_t* gae_file_destroy(gae_file_t* file)
{
	gae_file_close(file);
	gae_free(file->platform);
	return file;
}
