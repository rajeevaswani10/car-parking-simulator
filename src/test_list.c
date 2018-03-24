/*
 * test_list.c
 *
 *  Created on: Mar 21, 2018
 *      Author: rajeev
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "list.h"

static
void * test_random(void * arg){
	llist_t * l = (llist_t *)arg;


}

int main(){

	int data;
	LLIST_RC rc;

	llist_t * l = llist_create();

	assert(llist_isempty(l) == TRUE);
	assert(llist_size(l) == 0);

	int * ptr = (int *)malloc(sizeof(int));
	*ptr = 5;
	rc = llist_insert_first(l, (void *)ptr);
	assert(rc == LLIST_SUCCESS);

	ptr = (int *)malloc(sizeof(int));
	*ptr = 6;
	rc = llist_insert_first(l, (void *)ptr);
	assert(rc == LLIST_SUCCESS);

	ptr = (int *)malloc(sizeof(int));
	*ptr = 10;
	rc = llist_insert_last(l, (void *)ptr);
	assert(rc == LLIST_SUCCESS);

	ptr = (int *)malloc(sizeof(int));
	*ptr = 24;
	rc = llist_insert_last(l, (void *)ptr);
	assert(rc == LLIST_SUCCESS);

	ptr = llist_pop_element_from_front(l);
	assert(*ptr == 6);
	assert(llist_size(l) == 3);
	free(ptr);

	ptr = llist_pop_element_from_last(l);
	assert(*ptr == 24);
	assert(llist_size(l) == 2);
	free(ptr);


	ptr = llist_pop_element_from_front(l);
	assert(*ptr == 5);
	assert(llist_size(l) == 1);
	free(ptr);

	ptr = llist_pop_element_from_last(l);
	assert(rc == LLIST_SUCCESS);
	assert(*ptr == 10);
	assert(llist_size(l) == 0);
	free(ptr);


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
