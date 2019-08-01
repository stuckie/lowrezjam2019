#ifndef _GAE_DLIST_H_
#define _GAE_DLIST_H_

/* Double Linked List node */
typedef struct gae_dlist_node_s {
	void* data;
	
	struct gae_dlist_node_s* prev;
	struct gae_dlist_node_s* next;
} gae_dlist_node_t;

/* Double Linked List structure */
typedef struct gae_dlist_s {
	gae_dlist_node_t* start;
	gae_dlist_node_t* end;
} gae_dlist_t;

typedef void (*gae_dlist_foreach_t)(gae_dlist_node_t* node);

/* Initialise a new Double Linked List */
gae_dlist_t* gae_dlist_init(gae_dlist_t* list);

/* Add a node to this list */
gae_dlist_t* gae_dlist_add(gae_dlist_t* list, gae_dlist_node_t* node);

/* Insert a node after prev in this list */
gae_dlist_t* gae_dlist_insert(gae_dlist_t* list, gae_dlist_node_t* prev, gae_dlist_node_t* node);

/* For each node in this list, perform the given callback */
gae_dlist_t* gae_dlist_foreach(gae_dlist_t* list, gae_dlist_foreach_t foreach);

/* Remove the node from this list */
gae_dlist_t* gae_dlist_remove(gae_dlist_t* list, gae_dlist_node_t* node);

/* Destroy this list - does not free data pointers */
gae_dlist_t* gae_dlist_destory(gae_dlist_t* list);

/* Initialise a new Double Linked List node with the given data */
gae_dlist_node_t* gae_dlist_node_init(gae_dlist_node_t* node, void* data);

/* Destroy a Double Linked List node - does not free data pointer */
gae_dlist_node_t* gae_dlist_node_destroy(gae_dlist_node_t* node);

#endif
