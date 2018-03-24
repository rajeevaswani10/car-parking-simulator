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

typedef unsigned long 	uLong;

typedef struct llist_t  llist_t;


#define TRUE 	1
#define FALSE 	0

/**
* @brief creates list
*
*/
llist_t * llist_create();

/**
* @brief destroy list
*
*/
void llist_destroy(llist_t * list);

/**
* @brief check if list is empty.
*
* returns true if empty
*/
int llist_isempty(llist_t * list);

/**
* @brief inserts element in last
*
*
*/
LLIST_RC llist_insert_last(llist_t * list, void * data);

/**
* @brief inserts element at front
*
*
*/
LLIST_RC llist_insert_first(llist_t * list, void * data);

/**
* @brief removes element at specific position and returns
*
*
*/
void * llist_pop_element_at_pos(llist_t * list, unsigned int pos);

/**
* @brief removes element at front position and returns
*
*
*/
void * llist_pop_element_from_front(llist_t * list);

/**
* @brief removes element at last position and returns
*
*
*/
void * llist_pop_element_from_last(llist_t * list);

/**
* @brief removes element from random position and returns the same
*
*
*/
void * llist_pop_element_at_random(llist_t * list);

/**
* @brief returns number of element in list
*
*
*/
uLong llist_size(llist_t * list);


#endif
