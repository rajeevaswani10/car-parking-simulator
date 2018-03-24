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

#include <header/parking_buffer.h>


#define FREEIF(ptr)  if(ptr != NULL) free(ptr);


struct vehicle_info_t {
	uLong car_id;
};

typedef struct vehicle_info_t 	 vhinfo_t;


struct parking_slot_t {
	uLong id;
	vhinfo_t * vh;
};

typedef struct parking_slot_t	parking_slot_t ;


struct parking_buffer_t  {

	uLong capacity;
	uLong available;

	parking_slot_t * slots;
	pthread_mutex_t slots_mutx;

	parking_buffer_stats_t stats;
};

static
vhinfo_t * vhinfo_create(uLong car_id) {
	vhinfo_t * vh = (vhinfo_t *)malloc(sizeof(vhinfo_t));
	vh->car_id = car_id;
	return vh;
}

static
void vhinfo_destroy (vhinfo_t * vh) {
	FREEIF(vh);
}

static
void randomSleep(uLong maxDuration, uLong tid){
	int retval;
	time_t t;
	/* Intializes random number generator */
	srand((unsigned) time(&t) + tid); //to increase randomness per thread..

	uLong duration =  rand() % maxDuration + 1;
	//printf("tid %lu duration - %lu\n", tid,duration);
	usleep(duration);
}

static
parking_buffer_t * snapshot (parking_buffer_t * pb){
	int i;
	parking_buffer_t * tmp = pb_create(pb->capacity);

	pthread_mutex_lock(&(pb->slots_mutx));
	tmp->available = pb->available;
	tmp->stats = pb->stats;

	for(i=0; i<pb->capacity; i++){
		parking_slot_t * slot = &(pb->slots[i]);
		if( slot->vh!= NULL ){
			vhinfo_t * vh = vhinfo_create(slot->vh->car_id);
			parking_slot_t * tmp_slot = &(tmp->slots[i]) ;
			tmp_slot->vh = vh;
		}
	}

	pthread_mutex_unlock(&(pb->slots_mutx));
	return tmp;
}

parking_buffer_t * pb_create(uLong capacity)
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

PB_RC pb_park(parking_buffer_t * pb, uLong car_id, uLong * slot_id)
{
	int i;

	pthread_mutex_lock(&(pb->slots_mutx));

	//sleep for sometime in this critical section.
	randomSleep(200000, 0);

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

PB_RC pb_unpark(parking_buffer_t * pb, uLong car_id)
{
	int i;

	pthread_mutex_lock(&(pb->slots_mutx));

	//sleep for sometime in this critical section.
	randomSleep(200000, 0);

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

uLong pb_get_free_slots_count(parking_buffer_t * pb )
{
	return pb->available;
}

void pb_print(parking_buffer_t *parking_buffer){
	int i;

	parking_buffer_t * pb = snapshot(parking_buffer);

	int COLS_PER_ROW = 10;
	vhinfo_t * ptr;

	printf("\n\nCar Park Map:");
	printf("\n");
	printf("Capacity:  %lu | Occupied:  %lu | Available:  %lu | stats:  park_ok:%lu , park_err:%lu , unpark_err:%lu ",
			pb->capacity,(pb->capacity - pb->available),pb->available, (pb->stats).n_park_success, (pb->stats).n_park_failure,(pb->stats).n_unpark_failure);
	printf("\n\n");

	uLong capacity = pb->capacity;
	unsigned row = 0;
	while(capacity > 0){
		long n = (capacity > COLS_PER_ROW) ? COLS_PER_ROW : capacity;

		for(i=0; i<n; i++) printf("   %2lu   ",pb->slots[row*COLS_PER_ROW + i].id);
		printf("\n");

		for(i=0; i<n; i++) printf("--------");
		printf("\n");

		ptr = pb->slots[row*10].vh;
		if(ptr!=NULL)
			printf("[ %5lu",ptr->car_id);
		else
			printf("[      ");
		for(i=1; i<n; i++){
			ptr = pb->slots[row*10 + i].vh;
			if(ptr!=NULL)
				printf("| %6lu",ptr->car_id);
			else
				printf("| %6d",0);
		}

		printf(" ]");
		printf("\n");
		for(i=0; i<n; i++) printf("--------");
		printf("\n\n");

		capacity = capacity - n;
		row++;
	}
	printf("\n\n\n");
}

