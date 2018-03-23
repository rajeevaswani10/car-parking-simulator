
#ifndef _PARKING_BUFFER_H_
#define _PARKING_BUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    PB_SUCCESS = 0x00,           /**< Operating success */
    PB_NOT_FOUND,
    PB_PARKING_FULL,
    PB_PARKING_EMPTY,
    PB_ERROR                     /**< Error: Generic error*/
} PB_RC;


typedef struct parking_buffer_t    parking_buffer_t;


#define NOT_FOUND	-1	
#define EMPTY 		0	
#define OCCUPIED 	1	


parking_buffer_t * pb_create(unsigned int capacity, unsigned int in_valet_n, unsigned int out_valet_n);

void pb_destroy(parking_buffer_t * pb);

PB_RC pb_park(parking_buffer_t * pb, long car_id);

PB_RC pb_unpark(parking_buffer_t * pb, long car_id);

unsigned get_free_slots_count(parking_buffer_t * pb );

void pb_print(parking_buffer_t * pb );

#endif
