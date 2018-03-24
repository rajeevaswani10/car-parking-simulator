/*
 * test_queue.c
 *
 *  Created on: Mar 21, 2018
 *      Author: rajeev
 */

#include <stdio.h>
#include <assert.h>
#include <header/queue.h>

int main() {

	void * data = NULL;
	int rc;

	queue_t * q = queue_create();
	assert(queue_size(q) == 0);

	rc = queue_enqueue(q, (void *)5);
	assert(rc == 0);
	rc = queue_enqueue(q, (void *)6);
	assert(rc == 0);
	rc = queue_enqueue(q, (void *)10);
	assert(rc == 0);
	rc = queue_enqueue(q, (void *)24);
	assert(rc == 0);

	data = queue_dequeue(q);
	assert((int)data == 5);
	assert(queue_size(q) == 3);

	data = queue_dequeue(q);
	assert((int)data == 6);
	assert(queue_size(q) == 2);

	data = queue_dequeue(q);
	assert((int)data == 10);
	assert(queue_size(q) == 1);

	data = queue_dequeue(q);
	assert((int)data == 24);
	assert(queue_size(q) == 0);

	queue_destroy(q);

	printf("\n#####################\nALL TESTS PASS\n#####################\n");
}
