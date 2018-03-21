
#include "parking_buffer.h"
#include<stdlib.h>
#include<stdio.h>

struct parking_buffer_t  {
    int * buf;
    unsigned capacity;
    unsigned available;
};


parking_buffer_t * pb_create(unsigned int capacity)
{
	parking_buffer_t * pb = (parking_buffer_t *)malloc(sizeof(parking_buffer_t));
	int i;

	pb->capacity = capacity;
	pb->buf = (int *)malloc(sizeof(int)*capacity);
	pb->available = capacity;

	//init with default values.
	for(i=0; i<pb->capacity; i++) (pb->buf)[i]= EMPTY;
	return pb;
}

void pb_destroy(parking_buffer_t * pb)
{
	free(pb->buf);
}

PB_RETCODE pb_park(parking_buffer_t * pb, int car_id, int * slot)
{
	int i;
	for(i=0; i<pb->capacity; i++) {
		if( (pb->buf)[i] == EMPTY ){
			(pb->buf)[i] = car_id;
			*slot = i;
			pb->available--;
			return PB_SUCCESS;
		}
	}

	return PB_NOT_FOUND;
}

PB_RETCODE pb_unpark(parking_buffer_t * pb, int car_id, int slot)
{
	(pb->buf)[slot] = EMPTY;
	pb->available++;
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

