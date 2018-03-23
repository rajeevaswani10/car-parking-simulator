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

	rc = llist_insert_first(l, (void *)5);
	assert(rc == LLIST_SUCCESS);
	rc = llist_insert_first(l, (void *)6);
	assert(rc == LLIST_SUCCESS);
	rc = llist_insert_last(l, (void *)10);
	assert(rc == LLIST_SUCCESS);
	rc = llist_insert_last(l, (void *)24);
	assert(rc == LLIST_SUCCESS);

	data = (int)llist_pop_element_from_front(l);
	assert(data == 6);
	assert(llist_size(l) == 3);

	data = (int)llist_pop_element_from_last(l);
	assert(data == 24);
	assert(llist_size(l) == 2);


	data = (int)llist_pop_element_from_front(l);
	assert(data == 5);
	assert(llist_size(l) == 1);

	data = (int)llist_pop_element_from_last(l);
	assert(rc == LLIST_SUCCESS);
	assert(data == 10);
	assert(llist_size(l) == 0);


	//pop random test
	rc = llist_insert_last(l, (void *)5);
	rc = llist_insert_last(l, (void *)15);
	rc = llist_insert_last(l, (void *)5);
	rc = llist_insert_last(l, (void *)2);
	rc = llist_insert_last(l, (void *)1);
	rc = llist_insert_last(l, (void *)6);


	data = (int)llist_pop_element_at_random(l);
	printf("popped - %d\n", data);
	assert(llist_size(l) == 5);

	data = (int)llist_pop_element_at_random(l);
	printf("popped - %d\n", data);
	assert(llist_size(l) == 4);

	data = (int)llist_pop_element_at_random(l);
	printf("popped - %d\n", data);
	assert(llist_size(l) == 3);

	data = (int)llist_pop_element_at_random(l);
	printf("popped - %d\n", data);
	assert(llist_size(l) == 2);

	data = (int)llist_pop_element_at_random(l);
	printf("popped - %d\n", data);
	assert(llist_size(l) == 1);

	data = (int)llist_pop_element_at_random(l);
	printf("popped - %d\n", data);
	assert(llist_size(l) == 0);

	llist_destroy(l);

	printf("\n#####################\nALL TESTS PASS\n#####################\n");
}
