/**************************
 *	File: parking_buffer.c
 *
 *	Author: rajeev
 *
 * 	Desc:
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "parking_buffer.h"
#include "queue.h"


#define FREEIF(ptr)  (if(ptr != NULL) free(ptr))


struct vehicle_info_t {
	long car_id;
};

typedef vehicle_info_t 	 vhinfo_t;


struct parking_slot_t {
	unsigned int id;
	vhinfo_t * vh;
};

typedef struct parking_slot_t	parking_slot_t ;


struct parking_buffer_t  {

	unsigned int capacity;
	unsigned int in_valet_n;
	unsigned int out_valet_n;

	parking_slot_t * slots;
	pthread_mutex_t slots_mutx;

    unsigned available;
    pthread_mutex_t availble_mutx;

    queue_t * arrivals;
    queue_t * departures;

    pthread_t * in_valet_t;
    pthread_t * out_valet_t;

    int status; //1 running  0 stopped
};

static
vhinfo_t * vhinfo_create(long car_id) {
	vhinfo_t * vh = (vh_info_t *)malloc(sizeof(vh_info_t));
	vh->car_id = car_id;
	return vh;
}

static
void vhinfo_destroy (vhinfo_t * vh) {
	FREEIF(vh);
}

static
void * in_valet (void * arg) {
	pthread_t id  = pthread_self();
	parking_buffer_t * pb = (parking_buffer_t *)arg;
	int slot;
	int rc;

	while(pb->status){

		//todo
		sleep(1);
	}

}

static
void * out_valet (void * arg) {
	pthread_t id  = pthread_self();
	parking_buffer_t * pb = (parking_buffer_t *)arg;
	int slot;
	int rc;

	while(pb->status){

		//todo
		sleep(1);
	}
}

static
void start(parking_buffer_t * pb){
	int i;

	if(pb->status)
	{
		printf("already running.. \n");
		return;
	}

	pb->status = 1;

	for(i=0; i<pb->in_valet_n;i++)
	{
		int rc = pthread_create(&(pb->in_valet_t[i]), NULL, in_valet, (void *)pb);
		if(rc !=0 ) {
			printf("FATAL ERR: failed to create thread\n");
			return;
		}
	}

	for(i=0; i<pb->out_valet_n;i++)
	{
		int rc = pthread_create(&(pb->out_valet_t[i]), NULL, out_valet, (void *)pb);
		if(rc !=0 ) {
			printf("FATAL ERR: failed to create thread\n");
			return;
		}
	}
}

static
void stop(parking_buffer_t * pb){
	if( !pb->status)
	{
		printf("already stopped.. \n");
		return;
	}

	pb->status = 0; //all threads loop should end coz of this.

	//todo : wait for threads to stop here..
	printf("stopping.....\n");
	for(i=0; i<pb->in_valet_n;i++)
		pthread_join(pb->in_valet_t[i], NULL);
	for(i=0; i<pb->out_valet_n;i++)
		pthread_join(pb->out_valet_t[i], NULL);

}

parking_buffer_t * pb_create(unsigned int capacity, unsigned int in_valet_n, unsigned int out_valet_n)
{
	parking_buffer_t * pb = (parking_buffer_t *)malloc(sizeof(parking_buffer_t));
	int i;

	pb->capacity = capacity;
	pb->in_valet_n = in_valet_n;
	pb->out_valet_n = out_valet_n;

	pb->slots = (parking_slot_t *)malloc(sizeof(parking_slot_t)*capacity);
	pb->available = capacity;

	//init with default values.
	for(i=0; i<pb->capacity; i++) {
		pb->slots[i].id = i+1;
		pb->slots[i].vehicle_info = NULL;
	}

	pb->arrivals = queue_create();
	pb->departures = queue_create();

	pb->status = 0;

	pb->in_valet_t = (pthread_t *)malloc( sizeof(pthread_t)*in_valet_n );
	pb->out_valet_t = (pthread_t *)malloc( sizeof(pthread_t)*out_valet_n );

	start(pb);
	return pb;
}

void pb_destroy(parking_buffer_t * pb)
{
	stop();
	queue_destroy(arrivals);
	queue_destroy(departures);

	for(i=0; i<pb->capacity; i++) {
		vhinfo_destroy(pb->slots[i].vh);
	}
	FREEIF(pb->slots);
	FREEIF(in_valet_t);
	FREEIF(out_valet_t);
	FREEIF(pb);
}

void pb_start(parking_buffer_t * pb)
{
	start(pb);
}

void pb_stop(parking_buffer_t * pb)
{
	stop(pb);
}

PB_RC pb_park(parking_buffer_t * pb, int car_id)
{
	//todo
	return PB_SUCCESS;
}

PB_RC pb_unpark(parking_buffer_t * pb, int car_id)
{
	//todo
	return PB_SUCCESS;
}

unsigned int get_free_slots_count(parking_buffer_t * pb )
{
	return pb->available;
}

void pb_print(parking_buffer_t *pb){
	int i;

	printf("Car Park Map:");
	printf("\n");
	printf("Capacity:  %u | Occupied:  %u | Available:  %u ,", pb->capacity,(pb->capacity - pb->available),pb->available);
	printf("\n");

	for(i=0; i<pb->capacity; i++) printf("  %2d    ",i+1);
	printf("\n");

	for(i=0; i<pb->capacity; i++) printf("--------");
	printf("\n");

	printf("[ %5d",(pb->buf)[0]);
	for(i=1; i<pb->capacity; i++){
		printf(" | %5d",(pb->buf)[i]);
	}

	printf(" ]");
	printf("\n");
	for(i=0; i<pb->capacity; i++) printf("--------");
	printf("\n");
}

