/*
 * queue.c
 *
 *  Created on: Mar 21, 2018
 *      Author: rajeev
 *
 *      desc: queue based on list.
 */


#include <stdio.h>
#include <header/list.h>
#include <header/queue.h>


struct queue_t  {
	llist_t * list;
};

//create queue
queue_t * queue_create()
{
	queue_t * q = (queue_t * )malloc(sizeof(queue_t));
	q->list = llist_create();
	return q;
}

//destroy queue
void queue_destroy( queue_t * q )
{
	llist_destroy(q->list);
	free(q);
}

// To Enqueue an integer
int queue_enqueue(queue_t * q, void * data)
{
	return llist_insert_last(q->list, data);
}

// To Dequeue an integer.
void * queue_dequeue(queue_t * q)
{
	return llist_pop_element_from_front(q->list);
}

//size
unsigned int queue_size(queue_t * q)
{
	return llist_size(q->list);
}

