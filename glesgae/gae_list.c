#include "gae_list.h"

static int gae_list_contains(gae_list_t* const list, gae_list_node_t* const node)
{
	gae_list_node_t* current;
	
	current = list->start;
	
	if (node == list->start) return 1;
	if (node == list->end) return 1;
	
	while (0 != current) {
		if (node == current) return 1;
		current = current->next;
	}
	
	return 0;
}

gae_list_t* gae_list_init(gae_list_t* list)
{
	list->start = 0;
	list->end = 0;
	
	return list;
}

gae_list_t* gae_list_add(gae_list_t* list, gae_list_node_t* node)
{
	if (0 == list->start) list->start = node;
	if (0 != list->end) list->end->next = node;
	
	list->end = node;
	
	return list;
}

gae_list_t* gae_list_insert(gae_list_t* list, gae_list_node_t* prev, gae_list_node_t* node)
{
	#ifndef NDEBUG
		if (1 != gae_list_contains(list, prev)) return list;
	#endif
	
	node->next = prev->next;
	prev->next = node;
	
	if (prev == list->end)
		list->end = node;
	
	return list;
}

gae_list_t* gae_list_foreach(gae_list_t* list, gae_list_foreach_t foreach)
{
	gae_list_node_t* current;
	gae_list_node_t* next;
	
	current = list->start;
	
	while (0 != current) {
		next = current->next;
		(*foreach)(current);
		current = next;
	}
	
	return list;
}

gae_list_t* gae_list_remove(gae_list_t* list, gae_list_node_t* node)
{
	#ifndef NDEBUG
		if (1 != gae_list_contains(list, node)) return list;
	#endif
	
	if (node == list->start) {
		list->start = node->next;
	} else {
		gae_list_node_t* current = list->start;
		
		while (0 != current) {
			if (node == current->next) {
				current->next = node->next;
				break;
			}
		}
	}
	
	return list;
}

void gae_list_node_delete(gae_list_node_t* node)
{
	gae_list_node_destroy(node);
}

gae_list_t* gae_list_destroy(gae_list_t* list)
{
	gae_list_foreach(list, gae_list_node_delete);
	
	list->start = 0;
	list->end = 0;
	
	return list;
}

gae_list_node_t* gae_list_node_init(gae_list_node_t* node, void* data)
{
	node->data = data;
	node->next = 0;
	
	return node;
}

gae_list_node_t* gae_list_node_destroy(gae_list_node_t* node)
{
	node->data = 0;
	node->next = 0;
	
	return node;
}
