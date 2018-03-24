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
    unsigned available;

	parking_slot_t * slots;
	pthread_mutex_t slots_mutx;

	parking_buffer_stats_t stats;
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

parking_buffer_t * pb_create(unsigned int capacity)
{
	parking_buffer_t * pb = (parking_buffer_t *)malloc(sizeof(parking_buffer_t));
	int i;

	pb->capacity = capacity;
	pb->slots = (parking_slot_t *)malloc(sizeof(parking_slot_t)*capacity);
	pb->available = capacity;

	//init with default values.
	for(i=0; i<pb->capacity; i++) {
		pb->slots[i].id = i+1;
		pb->slots[i].vh = NULL;
	}

	//init stats
	pb->stats.n_park_success = 0;
	pb->stats.n_park_failure = 0;
	pb->stats.n_unpark_failure = 0;

	pthread_mutex_init(&(pb->slots_mutx), NULL);
	return pb;
}

void pb_destroy(parking_buffer_t * pb)
{
	int i;

	pthread_mutex_lock(&(pb->slots_mutx));
	for(i=0; i<pb->capacity; i++) {
		vhinfo_destroy(pb->slots[i].vh);
	}
	FREEIF(pb->slots);
	pthread_mutex_unlock(&(pb->slots_mutx));

	pthread_mutex_destroy(&(pb->slots_mutx));
	FREEIF(pb);
}

PB_RC pb_park(parking_buffer_t * pb, long car_id, unsigned int * slot_id)
{
	int i;

	pthread_mutex_lock(&(pb->slots_mutx));
	if(pb->available <= 0) {
		pb->stats.n_park_failure ++;
		pthread_mutex_unlock(&(pb->slots_mutx));
		return PB_PARKING_FULL;
	}

	vhinfo_t * vh = vhinfo_create(car_id);
	for(i=0; i<pb->capacity; i++){
		parking_slot_t * slot = &(pb->slots[i]);
		if(slot->vh == NULL){
			slot->vh = vh;
			*slot_id = slot->id;
			pb->available--;
			break;
		}
	}
	if(i == pb->capacity){
		pb->stats.n_park_failure ++;
		pthread_mutex_unlock(&(pb->slots_mutx));
		return PB_ERROR;
	}

	(pb->stats).n_park_success ++;
	pthread_mutex_unlock(&(pb->slots_mutx));
	return PB_SUCCESS;
}

PB_RC pb_unpark(parking_buffer_t * pb, long car_id)
{
	int i;

	pthread_mutex_lock(&(pb->slots_mutx));
	vhinfo_t * vh = NULL;
	for(i=0; i<pb->capacity; i++){
		parking_slot_t * slot = &(pb->slots[i]);
		if( slot->vh!= NULL && slot->vh->car_id == car_id ){
			vh = slot->vh;
			slot->vh = NULL;
			pb->available++;
			vhinfo_destroy(vh);
			break;
		}
	}
	if(i == pb->capacity){
		pb->stats.n_unpark_failure ++;
		pthread_mutex_unlock(&(pb->slots_mutx));
		return PB_NOT_FOUND;
	}

	pthread_mutex_unlock(&(pb->slots_mutx));
	return PB_SUCCESS;
}

unsigned int get_free_slots_count(parking_buffer_t * pb )
{
	return pb->available;
}

void pb_print(parking_buffer_t *pb){
	int i;
	vhinfo_t * ptr;

	printf("\n\nCar Park Map:");
	printf("\n");
	printf("Capacity:  %u | Occupied:  %u | Available:  %u | stats: %u , %u , %u ",
			pb->capacity,(pb->capacity - pb->available),pb->available, (pb->stats).n_park_success, (pb->stats).n_park_failure,(pb->stats).n_unpark_failure);
	printf("\n\n");

	long capacity = pb->capacity;
	unsigned row = 0;
	while(capacity > 0){
		long n = (capacity > 10)? 10 : capacity;

		for(i=0; i<n; i++) printf("  %2d    ",pb->slots[row*10 + i].id);
		printf("\n");

		for(i=0; i<n; i++) printf("--------");
		printf("\n");

		ptr = pb->slots[row*10].vh;
		if(ptr!=NULL)
			printf("[ %5ld",ptr->car_id);
		else
			printf("[      ");
		for(i=1; i<n; i++){
			ptr = pb->slots[row*10 + i].vh;
			if(ptr!=NULL)
				printf("| %6ld",ptr->car_id);
			else
				printf("|       ");
		}

		printf(" ]");
		printf("\n");
		for(i=0; i<n; i++) printf("--------");
		printf("\n");

		capacity = capacity - n;
		row++;
	}
	printf("\n\n\n");
}

