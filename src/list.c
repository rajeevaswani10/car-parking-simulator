
/*
 * File: list.c
 * Author: Rajeev
 *
 * Desc: thread safe list implementation in c
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list.h"

struct llist_node_t {
	int data;
	struct list_node_t * next;
};

typedef struct llist_node_t 	llist_node_t;


struct llist_t {
	llist_node_t * head;
	llist_node_t * tail;
	unsigned int count;

	pthread_mutex_t mutex;
};


static llist_node_t * create_node (int data){
	llist_node_t * node = (llist_node_t *)malloc(sizeof(llist_node_t));
	node->data  = data;
	node->next = null;
	return node;
}

llist_t * llist_create()
{
	llist_t * l = (llist_t *)malloc(sizeof(llist_t));
	l->head = l->tail = NULL;
	l->count = 0;
	pthread_mutex_init(&(l->mutex), NULL);

	return l;
}

void llist_destroy(llist_t * list)
{
	//todo
}

int llist_isempty(llist_t * list)
{
	if(list->head == NULL)
		return TRUE;
	else
		return FALSE;
}

LLIST_RC llist_insert_last(llist_t * list, int data)
{

	return LLIST_SUCCESS;
}

LLIST_RC llist_insert_first(llist_t * list, int data)
{
	llist_node_t * node = create_node(data);

	pthread_mutex_lock(&(l->mutex));


	return LLIST_SUCCESS;

}

LLIST_RC llist_insert_at_position(llist_t * list, int index, int data)
{

	return LLIST_SUCCESS;
}

LLIST_RC llist_delete_last(llist_t * list)
{

	return LLIST_SUCCESS;
}

LLIST_RC llist_delete_first(llist_t * list)
{

	return LLIST_SUCCESS;
}

LLIST_RC llist_delete_at_position(llist_t * list, int index)
{

	return LLIST_SUCCESS;
}

LLIST_RC llist_get_element(llist_t * list, unsigned index, int * data)
{

	return LLIST_SUCCESS;
}

LLIST_RC llist_pop_element_from_front(llist_t * list, int * data)
{

	return LLIST_SUCCESS;
}

LLIST_RC llist_pop_element_from_last(llist_t * list, int * data)
{

	return LLIST_SUCCESS;
}

void llist_print(llist_t * list)
{

}

unsigned int llist_size(llist_t * list)
{

	return LLIST_SUCCESS;
}

