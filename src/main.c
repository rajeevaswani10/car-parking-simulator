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
llist_t * departures;

pthread_t * in_valet_t;
pthread_t * out_valet_t;

pthread_t monitor_t;


static
long generate_car_id (long seed)
{
//	int retval;
//	time_t t;
//	/* Intializes random number generator */
//	srand(((unsigned) time(&t)) + seed);

//	return rand() % 99999 + 1000;
	static unsigned long n = 1000;
	return n++;
}

static
void randomSleep(){
	int retval;
	time_t t;
	/* Intializes random number generator */
	srand((unsigned) time(&t));

	usleep(rand() % 1000000);
}

static
void * in_valet (void * arg){
	static unsigned int id = 0;
	int rc;

	unsigned int myid = ++id;
	while(1){
		long car_id = generate_car_id(id);
		unsigned int slot;
		rc = pb_park(pb,car_id, &slot);
		if(rc == PB_SUCCESS) {
			printf("Car-in-valet %u - INFO - Car id: %ld in slot %u\n", myid, car_id, slot);
			llist_insert_last(departures, (void *)car_id);
		}
		else
		{
			printf("Car-in-valet %u - ERROR - Car id: %ld fail to park. retcode: %d\n", myid, car_id, rc);
		}

		randomSleep();
	}

}

static
void * out_valet (void * arg)
{
	static unsigned int id = 0;
	int rc;

	unsigned int myid = ++id;
	while(1){
		void * tmp = llist_pop_element_at_random(departures);
		if(tmp != NULL){
			long car_id = (long)tmp;
			rc = pb_unpark(pb,car_id);
			if(rc == PB_SUCCESS) {
				printf("Car-out-valet %u - INFO - Car id %ld has left\n", myid, car_id);
			}
			else
				printf("Car-out-valet %u - ERROR - Car id %ld fail to un-park. retcode %d\n", myid, car_id, rc);
		}
		randomSleep();
	}
}

static
void * monitor (void * arg)
{
	while(1){
		pb_print(pb);
		sleep(1);
	}
}


int main(int argc, char * argv[]){

	int rc;
	int i;
	printf("argc %d\n", argc);
	printf("argv %s\n", argv[1]);

	if(argc != 4) //including file name
	{
		printf("error : insufficient parameters\n usage for eg.: <program>  10 4 1 \n\n");
		return -1;
	}

	char * ptr;
	long capacity = strtol(argv[1], &ptr, 10);
	long n_invalet = strtol(argv[2], &ptr, 10);
	long n_outvalet = strtol(argv[3], &ptr, 10);


	pb = pb_create(capacity);
	departures = llist_create();

	in_valet_t = (pthread_t *)malloc(sizeof(pthread_t)*n_invalet);
	for(i=0;i<n_invalet;i++){
		rc = pthread_create(&in_valet_t[i], NULL, in_valet, NULL);
	}

	out_valet_t = (pthread_t *)malloc(sizeof(pthread_t)*n_outvalet);
	for(i=0; i<n_outvalet; i++){
		rc = pthread_create(&out_valet_t[i], NULL, out_valet, NULL);
	}

	rc= pthread_create(&monitor_t, NULL, monitor, NULL);


	pthread_join(monitor_t, &rc); //else main thread will go out.

	llist_destroy(departures);
	pb_destroy(pb);
}

