/*********
*  Car parking simulator
* Author: Rajeev
*
*/

#include<stdio.h>
#include<stdlib.h>

#include "parking_buffer.h"
#include "list.h"
#include "queue.h"


parking_buffer_t* pb ;
queue_t * arrivals;
llist_t * departures;

pthread_t in_valet_t[5];
pthread_t out_valet_t[2];

pthread_t request_generator;

pthread_t monitor_t;

void * in_valet (void * arg){
	int id;
	int slot;
	int rc;

	while(1){
		rc = queue_dequeue(arrivals, &id);
		if(rc != 0) {
			if (rc != LLIST_EMPTY)
			{
				printf("error fetching item.. \n");
				break;
			}
			else
				printf("queue empty\n");
		}
		else
		{
			rc = pb_park(pb,id,&slot);
			if(rc == 0) {
				printf("slot - %d\n", slot);
				llist_insert_last(departures, slot);
			}
			else
				printf("err: rc - %d\n", rc);
		}
		sleep(1);
	}

}

void * out_valet (void * arg)
{
	int id;
	int slot;
	int rc;

	while(1){
		rc = llist_pop_element_at_random(departures,&slot);
		if(rc != 0) {
			if (rc != LLIST_EMPTY)
			{
				printf("error fetching item.. \n");
				break;
			}
			else
				printf("queue empty\n");
		}
		else
		{
			rc = pb_unpark(pb,&id,slot);
			if(rc == 0) {
				printf("id - %d\n", id);
			}
			else
				printf("err: rc - %d\n", rc);
		}
		sleep(1);
	}
}

void * monitor (void * arg)
{
	while(1){
		pb_print(pb);
		sleep(1);
	}
}

void * parking_request_generator (void * arg)
{
	int retval;
	time_t t;

	/* Intializes random number generator */
	srand((unsigned) time(&t));

	while(1){
		int id = rand() % 9999 + 1;
		//printf("pushing item - %d\n", id);
	    retval = queue_enqueue(arrivals, id);
		sleep(1);
	}

}


int main(){

	int rc;
	int i;

	pb = pb_create(15);
	arrivals = queue_create();
	departures = llist_create();

	for(i=0;i<1;i++){
		rc = pthread_create(&in_valet_t[i], NULL, in_valet, NULL);
	}

	for(i=0;i<1;i++){
		//rc = pthread_create(&out_valet_t[i], NULL, out_valet, NULL);
	}

	rc= pthread_create(&request_generator, NULL, parking_request_generator, NULL);

	rc= pthread_create(&monitor_t, NULL, monitor, NULL);


	pthread_join(monitor_t, &rc); //else main thread will go out.

	queue_destroy(arrivals);
	llist_destroy(departures);
	pb_destroy(pb);
}

