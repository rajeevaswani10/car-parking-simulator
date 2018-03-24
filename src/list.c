
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
	void * data;
	struct llist_node_t * next;
};

typedef struct llist_node_t 	llist_node_t;


struct llist_t {
	llist_node_t * head;
	llist_node_t * tail;
	unsigned int count;

	pthread_mutex_t mutex;
};


static
llist_node_t * create_node (void * data){
	llist_node_t * node = (llist_node_t *)malloc(sizeof(llist_node_t));
	node->data  = data;
	node->next = NULL;
	return node;
}

static
void * pop_element_at_pos(llist_t * l, unsigned int pos)
{
	void * data;
	llist_node_t * node = NULL;

	if(pos<0 || pos>=l->count) {
		return NULL;
	}

	if(pos == 0) { //pop first element
		node = l->head;
		if(l->head == l->tail ) { // meaning only one node in list..
			l->head = l->tail = NULL;
		} else {
			l->head = node->next;
		}
	} else if( pos == (l->count-1) ) { //pop last element
		llist_node_t * iter ;
		for(iter = l->head ; iter->next != l->tail ; iter = iter->next) ;  // find parent node of tail..
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

	data = node->data;
	free(node);
	return data;
}

llist_t * llist_create()
{
	llist_t * l = (llist_t *)malloc(sizeof(llist_t));
	l->head = l->tail = NULL;
	l->count = 0;
	pthread_mutex_init(&(l->mutex), NULL);

	return l;
}

void llist_destroy(llist_t * l)
{
	llist_node_t * node = NULL;

	pthread_mutex_lock(&(l->mutex));
	while(l->head != NULL) {
		node = l->head;
		l->head = l->head->next;
		free(node);
	}
	l->count = 0;
	pthread_mutex_unlock(&(l->mutex));

	pthread_mutex_destroy(&(l->mutex));
	free(l);
}

int llist_isempty(llist_t * l)
{
	if(l->head == NULL)
		return TRUE;
	else
		return FALSE;
}

LLIST_RC llist_insert_last(llist_t * l, void * data)
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

LLIST_RC llist_insert_first(llist_t * l, void * data)
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

void * llist_pop_element_at_pos(llist_t * l, unsigned int pos)
{
	void * data;

	pthread_mutex_lock(&(l->mutex));

	if(l->head == NULL){
		pthread_mutex_unlock(&(l->mutex));
		return NULL;
	}

	if(pos<0 || pos>=l->count) {
		pthread_mutex_unlock(&(l->mutex));
		return NULL;
	}

	data = pop_element_at_pos(l,pos);
	pthread_mutex_unlock(&(l->mutex));

	return data;
}

void * llist_pop_element_from_front(llist_t * l)
{
	return llist_pop_element_at_pos(l,0);
}

void * llist_pop_element_from_last(llist_t * l)
{
	return llist_pop_element_at_pos(l,(l->count-1));
}

void * llist_pop_element_at_random(llist_t * l)
{
	void * data;

	pthread_mutex_lock(&(l->mutex));

	if(l->head == NULL){
		pthread_mutex_unlock(&(l->mutex));
		return NULL;
	}

	/* generate random pos */
	time_t t;
	srand((unsigned) time(&t));
	int pos = rand() % l->count ;

	if(pos<0 || pos>=l->count) {
		pthread_mutex_unlock(&(l->mutex));
		return NULL;
	}

	data = pop_element_at_pos(l,pos);
	pthread_mutex_unlock(&(l->mutex));

	return data;
}

unsigned int llist_size(llist_t * l)
{
	return l->count;
}

