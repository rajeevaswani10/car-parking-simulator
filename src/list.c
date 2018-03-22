
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
	llist_node_t * node = create_node(data);

	pthread_mutex_lock(&(l->mutex));

	if(l->head == NULL) {
		l->head = l->tail = node;
	} else {
		l->tail->next = node;
		l->tail = node;
	}

	l->count++;
	pthread_mutex_unlock(&(l->mutex));
	return LLIST_SUCCESS;
}

LLIST_RC llist_insert_first(llist_t * l, int data)
{
	llist_node_t * node = create_node(data);

	pthread_mutex_lock(&(l->mutex));

	if(l->head == NULL) {
		l->head = l->tail = node;
	} else {
		node->next = l->head;
		l->head = node;
	}

	l->count++;
	pthread_mutex_unlock(&(l->mutex));
	return LLIST_SUCCESS;
}

LLIST_RC llist_pop_element_at_pos(llist_t * l, unsigned int pos, int * data)
{
	llist_node_t * node = NULL;

	if(l->head == NULL) return LLIST_EMPTY;

	if(pos<0 || pos>(l->count-1))  return LLIST_OUT_OF_BOUNDS;

	pthread_mutex_lock(&(l->mutex));

	if(pos == 0) { //pop first element
		node = l->head;
		if(l->head == l->tail ) { // meaning only one node in list..
			l->head = l->tail = NULL;
		} else {
			l->head = node->next;
		}
	} else (pos == (l->count-1) ) { //pop last element
		llist_node_t * iter ;
		for(iter = l->head ; iter->next == l->tail ; iter = iter->next) ;  // find parent node of tail..
		node = l->tail;
		iter->next = NULL;
		l->tail = iter;
	} else {
		llist_node_t *iter, *iter1  ;
		int i;
		for(i=0, iter = iter1 = l->head ; i < pos ; i++, iter=iter1, iter1 = iter1->next) ;  // find node at pos
		iter->next = iter1->next;
		iter1->next = NULL;
		node = iter1;
	}

	l->count--;
	pthread_mutex_unlock(&(l->mutex));

	*data = node->data;
	free(node);
	return LLIST_SUCCESS;
}

LLIST_RC llist_pop_element_from_front(llist_t * l, int * data)
{
	return llist_pop_element_at_pos(l,0,data);
}

LLIST_RC llist_pop_element_from_last(llist_t * l, int * data)
{
	return llist_pop_element_at_pos(l,(l->count-1),data);
}

LLIST_RC llist_pop_element_at_random(llist_t * l, int * data)
{
	/* generate random pos */
	time_t t;
	srand((unsigned) time(&t));
	int pos = rand() % l->count ;

	return llist_pop_element_at_pos(l, pos, data);
}

void llist_print(llist_t * l)
{
	llist_node_t * iter ;
	for(iter = l->head ; iter != NULL; iter = iter->next)
		printf("%d  ",iter->data);
}

unsigned int llist_size(llist_t * l)
{
	return l->count;
}

