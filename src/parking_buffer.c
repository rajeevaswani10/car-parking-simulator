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


#define FREEIF(ptr)  if(ptr != NULL) free(ptr);


struct vehicle_info_t {
	long car_id;
};

typedef struct vehicle_info_t 	 vhinfo_t;


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
	vhinfo_t * vh = (vhinfo_t *)malloc(sizeof(vhinfo_t));
	vh->car_id = car_id;
	return vh;
}

static
void vhinfo_destroy (vhinfo_t * vh) {
	FREEIF(vh);
}

static
int request_for_parking(parking_buffer_t * pb, long car_id){
	vhinfo_t * vh = vhinfo_create(car_id);
	return queue_enqueue(pb->arrivals, vh);
}

static
void * in_valet (void * arg) {
	pthread_t id  = pthread_self();
	parking_buffer_t * pb = (parking_buffer_t *)arg;
	vhinfo_t * vh;
	int i;

	while(pb->status){
		vh = queue_dequeue(pb->arrivals);
		if(vh != NULL) {
			for(i=0 ; i<pb->capacity ; i++){
				parking_slot_t * slot = &(pb->slots[i]);
				if(slot->vh == NULL){
					slot->vh = vh;
					break;
				}
			}
			if(i == pb->capacity)
				printf("ERROR: parking not found\n" );
		}

		sleep(1);
	}

	printf("thread id:%lu : stopped\n", id);
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

	printf("thread id:%lu : stopped\n", id);
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
	int i;
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

	printf("STOPPED\n");
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
		pb->slots[i].vh = NULL;
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
	int i;

	stop(pb);
	queue_destroy(pb->arrivals);
	queue_destroy(pb->departures);

	for(i=0; i<pb->capacity; i++) {
		vhinfo_destroy(pb->slots[i].vh);
	}
	FREEIF(pb->slots);
	FREEIF(pb->in_valet_t);
	FREEIF(pb->out_valet_t);
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

PB_RC pb_park(parking_buffer_t * pb, long car_id)
{
	return request_for_parking(pb,car_id);
}

PB_RC pb_unpark(parking_buffer_t * pb, long car_id)
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
	vhinfo_t * ptr;

	printf("Car Park Map:");
	printf("\n");
	printf("Capacity:  %u | Occupied:  %u | Available:  %u ,", pb->capacity,(pb->capacity - pb->available),pb->available);
	printf("\n");

	for(i=0; i<pb->capacity; i++) printf("  %2d    ",pb->slots[i].id);
	printf("\n");

	for(i=0; i<pb->capacity; i++) printf("--------");
	printf("\n");

	ptr = pb->slots[0].vh;
	if(ptr!=NULL)
		printf("[ %5ld",ptr->car_id);
	else
		printf("[      ");
	for(i=1; i<pb->capacity; i++){
		ptr = pb->slots[i].vh;
		if(ptr!=NULL)
			printf("| %6ld",ptr->car_id);
		else
			printf("|       ");
	}

	printf(" ]");
	printf("\n");
	for(i=0; i<pb->capacity; i++) printf("--------");
	printf("\n");
}

