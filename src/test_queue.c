/*
 * test_queue.c
 *
 *  Created on: Mar 21, 2018
 *      Author: rajeev
 */

#include <stdio.h>
#include "assert.h"
#include "queue.h"

int main() {

	int data;
	int rc;

	queue_t * q = queue_create();
	assert(queue_size(q) == 0);

	rc = queue_enqueue(q, 5);
	assert(rc == 0);
	rc = queue_enqueue(q, 6);
	assert(rc == 0);
	rc = queue_enqueue(q, 10);
	assert(rc == 0);
	rc = queue_enqueue(q, 24);
	assert(rc == 0);

	queue_print(q);

	rc = queue_dequeue(q,&data);
	assert(rc == 0);
	assert(data == 5);
	assert(queue_size(q) == 3);

	rc = queue_dequeue(q,&data);
	assert(rc == 0);
	assert(data == 6);
	assert(queue_size(q) == 2);


	rc = queue_dequeue(q,&data);
	assert(rc == 0);
	assert(data == 10);
	assert(queue_size(q) == 1);

	rc = queue_dequeue(q,&data);
	assert(rc == 0);
	assert(data == 24);
	assert(queue_size(q) == 0);

	queue_destroy(q);

	printf("\n#####################\nALL TESTS PASS\n#####################\n");
}
