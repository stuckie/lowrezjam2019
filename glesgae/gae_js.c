#include "gae_types.h"
#include "gae_js.h"
#include "gae_buffer.h"
#include "gae_file.h"
#include "gae_memory.h"

#include <string.h>

#define JSON_TAG_LF 10
#define JSON_TAG_CR 13
#define JSON_TAG_SPACE 32
#define JSON_TAG_TAB 9
#define JSON_TAG_QUOTE 34
#define JSON_TAG_OBJECT_START 123 /* { */
#define JSON_TAG_OBJECT_END 125 /* } */
#define JSON_TAG_ARRAY_START 91 /* [ */
#define JSON_TAG_ARRAY_END 93 /* ] */
#define JSON_TAG_COLON 58
#define JSON_TAG_COMMA 44
#define JSON_TAG_MINUS 45
#define JSON_TAG_PERIOD 46
#define JSON_TAG_0 48
#define JSON_TAG_1 49
#define JSON_TAG_2 50
#define JSON_TAG_3 51
#define JSON_TAG_4 52
#define JSON_TAG_5 53
#define JSON_TAG_6 54
#define JSON_TAG_7 55
#define JSON_TAG_8 56
#define JSON_TAG_9 57
#define JSON_TAG_f 102 /* _f_alse */
#define JSON_TAG_F 70 /* _F_alse */
#define JSON_TAG_n 110 /* _n_ull */
#define JSON_TAG_N 78 /* _N_ull */
#define JSON_TAG_t 116 /* _t_rue */
#define JSON_TAG_T 84 /* _T_rue */

static gae_json_node_t* gae_json_parse_node(gae_json_document_t* doc);

static gae_json_node_t* new_node()
{
	gae_json_node_t* next = gae_alloc(sizeof(gae_json_node_t));
	if (0 != next) {
		next->name.start = 0;
		next->name.end = 0;
		next->name.type = json_type_null;
		next->value.start = 0;
		next->value.end = 0;
		next->value.type = json_type_null;
		next->child = 0;
		next->sibling = 0;
#if defined GAE_JSON_DEBUG
		next->name.debugString = 0;
		next->value.debugString = 0;
#endif
	}
	
	return next;
}

static void string_set(gae_json_string_t* string, gae_json_string_type type, gae_buffer_t* buffer)
{
	char c = buffer->data[buffer->pos];
	string->type = type;
	string->start = buffer->pos;
	while ((json_type_string == type ? JSON_TAG_QUOTE != c :
		(	JSON_TAG_COMMA != c
		&&	JSON_TAG_ARRAY_END != c
		&&	JSON_TAG_OBJECT_END != c
		&&	JSON_TAG_CR != c
		&&	JSON_TAG_LF != c
		&&	JSON_TAG_SPACE != c
		&&	JSON_TAG_TAB != c
		))
		&& buffer->pos < buffer->length) {
		c = buffer->data[++buffer->pos];
	}
	string->end = buffer->pos;
#if defined GAE_JSON_DEBUG
	{
		unsigned int size = string->end - string->start;
		string->debugString = gae_alloc(sizeof(char) * size + 1);
		memset(string->debugString, 0, size + 1);
		memcpy(string->debugString, &(buffer->data[string->start]), size);
	}
#endif
}

static void add_child(gae_json_node_t* parent, gae_json_node_t* child)
{
	if (0 != parent) {
		if (0 == parent->child)
			parent->child = child;
		else {
			gae_json_node_t* sibling = parent->child->sibling;
			if (0 == sibling) parent->child->sibling = child;
			else {
				while (0 != sibling->sibling)
					sibling = sibling->sibling;
				sibling->sibling = child;
			}
		}
	}
}

gae_json_document_t* gae_json_document_init(gae_json_document_t* doc, const char* path)
{
	gae_file_t file;
	
	gae_file_init(&file, GAE_FILE_MODE_READ, GAE_FILE_TYPE_ASCII);
	gae_file_open(&file, path);
	if (GAE_FILE_NO_ERROR != file.status) return 0;
	if (0 == gae_buffer_from_file(&(doc->buffer), &file)) {
		gae_file_destroy(gae_file_close(&file));
		gae_buffer_destroy(&(doc->buffer));
		return 0;
	}
	
	gae_file_destroy(gae_file_close(&file));
	
	return doc;
}

gae_json_document_t* gae_json_document_parse(gae_json_document_t* doc)
{
	doc->root = gae_json_parse_node(doc);
	
	return doc;
}

static gae_json_node_t* gae_json_parse_node(gae_json_document_t* doc)
{
	gae_json_node_t* node = 0;
	int isParsing = 1;
	
	while (isParsing) {
		char c = doc->buffer.data[doc->buffer.pos++];
		switch (c) {
			case JSON_TAG_QUOTE: {
				if (0 == node) node = new_node();
				if (0 == node->name.start) string_set(&node->name, json_type_string, &doc->buffer);
				else if (0 == node->value.start) {
					string_set(&node->value, json_type_string, &doc->buffer);
					node->type = json_type_value;
				}
				doc->buffer.pos++; /* get past the end quote */
			}
			break;
			case JSON_TAG_0:
			case JSON_TAG_1:
			case JSON_TAG_2:
			case JSON_TAG_3:
			case JSON_TAG_4:
			case JSON_TAG_5:
			case JSON_TAG_6:
			case JSON_TAG_7:
			case JSON_TAG_8:
			case JSON_TAG_9: 
			case JSON_TAG_MINUS: {
				if (0 == node) node = new_node();
				doc->buffer.pos--; /* back up to consume the token as part of the string */
				string_set(&node->value, json_type_number, &doc->buffer);
				node->type = json_type_value;
			}
			break;
			case JSON_TAG_T:
			case JSON_TAG_t:
			case JSON_TAG_F:
			case JSON_TAG_f: {
				if (0 == node) node = new_node();
				doc->buffer.pos--; /* back up to consume the token as part of the string */
				string_set(&node->value, json_type_bool, &doc->buffer);
				node->type = json_type_value;
			}
			break;
			case JSON_TAG_N:
			case JSON_TAG_n: {
				if (0 == node) node = new_node();
				doc->buffer.pos--; /* back up to consume the token as part of the string */
				string_set(&node->value, json_type_null, &doc->buffer);
				node->type = json_type_value;
			}
			break;
			case JSON_TAG_COMMA: {
				return node;
			}
			break;
			case JSON_TAG_ARRAY_START: {
				gae_json_node_t* child = gae_json_parse_node(doc);

				if (0 == node) node = new_node();
				node->type = json_type_array;

				while (0 != child) {
					add_child(node, child);
					child = gae_json_parse_node(doc);
				}
				return node;
			}
			break;
			case JSON_TAG_OBJECT_START: {
				gae_json_node_t* child = gae_json_parse_node(doc);
				
				if (0 == node) node = new_node();
				node->type = json_type_object;
				
				while (0 != child) {
					add_child(node, child);
					child = gae_json_parse_node(doc);
				}
				return node;
			}
			break;
			case JSON_TAG_ARRAY_END: {
				return node;
			}
			case JSON_TAG_OBJECT_END: {
				return node;
			}
			break;
			default:
			break;
		}
		isParsing = (doc->buffer.pos < doc->buffer.length);
	};
	
	return node;
}

static void node_destroy(gae_json_node_t* node)
{
	gae_json_node_t* child = node->child;
	gae_json_node_t* sibling = node->sibling;
	
	if (0 != sibling) node_destroy(sibling);
	if (0 != child) node_destroy(child);
	
#if defined GAE_JSON_DEBUG
	if (0 != node->name.debugString) gae_free(node->name.debugString);
	if (0 != node->value.debugString) gae_free(node->value.debugString);
#endif
	
	gae_free(node);
}

gae_json_document_t* gae_json_document_destroy(gae_json_document_t* doc)
{
	node_destroy(doc->root);
	
	doc->root = 0;
	gae_buffer_destroy(&(doc->buffer));
	
	return doc;
}

char* gae_json_string(gae_json_document_t* doc, gae_json_string_t* string)
{
	unsigned long size = string->end - string->start;
	char* jstring = gae_alloc(sizeof(char) * size + 1);
	memset(jstring, 0, size + 1);
	memcpy(jstring, &(doc->buffer.data[string->start]), size);
	return jstring;
}

static int string_compare(gae_json_document_t* doc, gae_json_string_t* jstring, const char* cstring)
{
	unsigned long size = jstring->end - jstring->start;
	if (0 == size) return -1;
	return strncmp((char*)&(doc->buffer.data[jstring->start]), cstring, size);
}

gae_json_node_t* gae_json_node_find(gae_json_document_t* doc, gae_json_node_t* from, const char* name)
{
	gae_json_node_t* found = 0;
	
	if (0 == from) return 0;
	
	if (0 == string_compare(doc, &(from->name), name))
		return from;
	
	found = gae_json_node_find(doc, from->child, name);
	if (0 != found) return found;
	
	found = gae_json_node_find(doc, from->sibling, name);
	if (0 != found) return found;
	
	return 0;
}
