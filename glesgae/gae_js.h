#ifndef _GAE_JS_H_
#define _GAE_JS_H_

#include "gae_buffer.h"

/* adds strings to gae_json_string structs to aid debugging */
/* #define GAE_JSON_DEBUG 1 */

typedef enum gae_json_type_e
{	json_type_object
,	json_type_array
,	json_type_value
} gae_json_type;

typedef enum gae_json_string_type_e
{	json_type_number	/* all numbers are doubles */
,	json_type_string	/* does not include quotes */
,	json_type_bool		/* technically could just be a number */
,	json_type_null		/* null */
} gae_json_string_type;

typedef struct gae_json_string_s {
	unsigned long start;
	unsigned long end;
	gae_json_string_type type;
	
#if defined GAE_JSON_DEBUG
	char* debugString;
#endif
} gae_json_string_t;

typedef struct gae_json_node_s {
	gae_json_string_t name;
	gae_json_string_t value;
	gae_json_type type;
	
	struct gae_json_node_s* child;		/* parent -> child */
	struct gae_json_node_s* sibling;	/* child -> child */
} gae_json_node_t;

typedef struct gae_json_document_s {
	gae_buffer_t buffer;
	gae_json_node_t* root;
} gae_json_document_t;

gae_json_document_t* gae_json_document_init(gae_json_document_t* doc, const char* path);

gae_json_document_t* gae_json_document_parse(gae_json_document_t* doc);

gae_json_document_t* gae_json_document_destroy(gae_json_document_t* doc);

gae_json_node_t* gae_json_node_find(gae_json_document_t* doc, gae_json_node_t* from, const char* name);

char* gae_json_string(gae_json_document_t* doc, gae_json_string_t* string);

#endif
