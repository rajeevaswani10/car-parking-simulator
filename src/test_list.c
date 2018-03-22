/*
 * test_list.c
 *
 *  Created on: Mar 21, 2018
 *      Author: rajeev
 */

#include <stdio.h>
#include <assert.h>
#include "list.h"



int main(){

	int data;
	LLIST_RC rc;

	llist_t * l = llist_create();

	assert(llist_isempty(l) == TRUE);
	assert(llist_size(l) == 0);

	rc = llist_insert_first(l, 5);
	assert(rc == LLIST_SUCCESS);
	rc = llist_insert_first(l, 6);
	assert(rc == LLIST_SUCCESS);
	rc = llist_insert_last(l, 10);
	assert(rc == LLIST_SUCCESS);
	rc = llist_insert_last(l, 24);
	assert(rc == LLIST_SUCCESS);

	llist_print(l);

	rc = llist_pop_element_from_front(l,&data);
	assert(rc == LLIST_SUCCESS);
	assert(data == 6);
	assert(llist_size(l) == 3);

	rc = llist_pop_element_from_last(l,&data);
	assert(rc == LLIST_SUCCESS);
	assert(data == 24);
	assert(llist_size(l) == 2);


	rc = llist_pop_element_from_front(l,&data);
	assert(rc == LLIST_SUCCESS);
	assert(data == 5);
	assert(llist_size(l) == 1);

	rc = llist_pop_element_from_last(l,&data);
	assert(rc == LLIST_SUCCESS);
	assert(data == 10);
	assert(llist_size(l) == 0);


	//pop random test
	rc = llist_insert_last(l, 5);
	rc = llist_insert_last(l, 15);
	rc = llist_insert_last(l, 5);
	rc = llist_insert_last(l, 2);
	rc = llist_insert_last(l, 1);
	rc = llist_insert_last(l, 6);

	llist_print(l);

	rc = llist_pop_element_at_random(l,&data);
	assert(rc == LLIST_SUCCESS);
	printf("popped - %d\n", data);
	assert(llist_size(l) == 5);

	rc = llist_pop_element_at_random(l,&data);
	assert(rc == LLIST_SUCCESS);
	printf("popped - %d\n", data);
	assert(llist_size(l) == 4);

	rc = llist_pop_element_at_random(l,&data);
	assert(rc == LLIST_SUCCESS);
	printf("popped - %d\n", data);
	assert(llist_size(l) == 3);

	rc = llist_pop_element_at_random(l,&data);
	assert(rc == LLIST_SUCCESS);
	printf("popped - %d\n", data);
	assert(llist_size(l) == 2);

	rc = llist_pop_element_at_random(l,&data);
	assert(rc == LLIST_SUCCESS);
	printf("popped - %d\n", data);
	assert(llist_size(l) == 1);

	rc = llist_pop_element_at_random(l,&data);
	assert(rc == LLIST_SUCCESS);
	printf("popped - %d\n", data);
	assert(llist_size(l) == 0);

	llist_destroy(l);

	printf("\n#####################\nALL TESTS PASS\n#####################\n");
}
