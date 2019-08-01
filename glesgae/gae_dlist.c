#include "gae_dlist.h"

static int gae_dlist_contains(gae_dlist_t* const list, gae_dlist_node_t* const node)
{
	gae_dlist_node_t* current;
	
	if (0 == list) return -1;
	if (0 == node) return -1;
	
	current = list->start;
	
	if (node == list->start) return 1;
	if (node == list->end) return 1;
	
	while (current != list->end) {
		if (node == current) return 1;
		current = current->next;
	}
	
	return 0;
}

gae_dlist_t* gae_dlist_init(gae_dlist_t* list)
{
	list->start = 0;
	list->end = 0;
	
	return list;
}

gae_dlist_t* gae_dlist_add(gae_dlist_t* list, gae_dlist_node_t* node)
{
	if (0 == list->start) list->start = node;
	if (0 != list->end) {
		node->prev = list->end;
		list->end->next = node;
	}
	
	list->end = node;
	
	return list;
}

gae_dlist_t* gae_dlist_insert(gae_dlist_t* list, gae_dlist_node_t* prev, gae_dlist_node_t* node)
{
	#ifndef NDEBUG
		if (1 != gae_dlist_contains(list, prev)) return list;
	#endif
	
	node->next = prev->next;
	prev->next = node;
	
	node->prev = prev;
	node->next->prev = node;
	
	if (prev == list->end)
		list->end = node;
	
	return list;
}

gae_dlist_t* gae_dlist_foreach(gae_dlist_t* list, gae_dlist_foreach_t foreach)
{
	gae_dlist_node_t* current;
	gae_dlist_node_t* next;
	
	if (0 == list) return list;
	if (0 == foreach) return list;
	
	current = list->start;
	
	while (current != list->end) {
		next = current->next;
		(*foreach)(current);
		current = next;
	}
	
	return list;
}

gae_dlist_t* gae_dlist_remove(gae_dlist_t* list, gae_dlist_node_t* node)
{
	#ifndef NDEBUG
		if (1 != gae_dlist_contains(list, node)) return list;
	#endif
	
	if (node == list->start) {
		list->start = node->next;
		list->start->prev = 0;
	} else if (node == list->end) {
		list->end = node->prev;
		list->end->next = 0;
	} else {
		gae_dlist_node_t* prev = node->prev;
		gae_dlist_node_t* next = node->next;
		
		prev->next = node->next;
		next->prev = prev;
	}
	
	return list;
}

static void gae_dlist_node_delete(gae_dlist_node_t* node)
{
	gae_dlist_node_destroy(node);
}

gae_dlist_t* gae_dlist_destroy(gae_dlist_t* list)
{
	gae_dlist_foreach(list, gae_dlist_node_delete);
	
	return list;
}

gae_dlist_node_t* gae_dlist_node_init(gae_dlist_node_t* node, void* data)
{
	node->data = data;
	
	return node;
}

gae_dlist_node_t* gae_dlist_node_destroy(gae_dlist_node_t* node)
{
	node->data = 0;
	node->next = 0;
	node->prev = 0;
	
	return node;
}
