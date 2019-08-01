#ifndef _GAE_LIST_H_
#define _GAE_LIST_H_

/* Single Linked List node */
typedef struct gae_list_node_s {
	void* data;
	
	struct gae_list_node_s* next;
} gae_list_node_t;

/* Single Linked List structure */
typedef struct gae_list_s {
	gae_list_node_t* start;
	gae_list_node_t* end;
} gae_list_t;

typedef void (*gae_list_foreach_t)(gae_list_node_t* node);

/* Initialise a new Single Linked List */
gae_list_t* gae_list_init(gae_list_t* list);

/* Add a node to this list */
gae_list_t* gae_list_add(gae_list_t* list, gae_list_node_t* node);

/* Insert a node after prev in this list */
gae_list_t* gae_list_insert(gae_list_t* list, gae_list_node_t* prev,
 gae_list_node_t* node);

/* For each node in this list, do the given callback */
gae_list_t* gae_list_foreach(gae_list_t* list, gae_list_foreach_t foreach);

/* Remove the node from this list */
gae_list_t* gae_list_remove(gae_list_t* list, gae_list_node_t* node);

/* Destroy this list - does not free data pointers */
gae_list_t* gae_list_destroy(gae_list_t* list);

/* Initialise a new list node with the given data */
gae_list_node_t* gae_list_node_init(gae_list_node_t* node, void* data);

/* Delete the list node - does not delete data pointer */
gae_list_node_t* gae_list_node_destroy(gae_list_node_t* node);

#endif
