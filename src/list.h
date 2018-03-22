/*
 * list.h
 *
 *  Created on: Mar 21, 2018
 *      Author: rajeev
 */

#ifndef _LLIST_H_
#define _LLIST_H_

typedef enum
{
    LLIST_SUCCESS = 0x00,           /**< Operating success */
    LLIST_ITEM_NOT_FOUND,
    LLIST_OUT_OF_BOUNDS,
    LLIST_EMPTY,
    LLIST_ERROR                     /**< Error: Generic error*/
} LLIST_RC;

typedef struct llist_t  llist_t;


#define TRUE 	1
#define FALSE 	0


llist_t * llist_create();

void llist_destroy(llist_t * list);

int llist_isempty(llist_t * list);

LLIST_RC llist_insert_last(llist_t * list, int data);

LLIST_RC llist_insert_first(llist_t * list, int data);

LLIST_RC llist_get_element(llist_t * list, unsigned index, int * data);

LLIST_RC llist_pop_element_from_front(llist_t * list, int * data);

LLIST_RC llist_pop_element_from_last(llist_t * list, int * data);

LLIST_RC llist_pop_element_at_random(llist_t * list, int * data);

void llist_print(llist_t * list);

unsigned int llist_size(llist_t * list);


#endif
