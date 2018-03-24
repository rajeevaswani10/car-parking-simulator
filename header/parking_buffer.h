/*
 * parking_buffer.h
 *
 *  Created on: Mar 21, 2018
 *      Author: rajeev
 */

#ifndef _PARKING_BUFFER_H_
#define _PARKING_BUFFER_H_

typedef enum
{
    PB_SUCCESS = 0x00,           /**< Operating success */
    PB_NOT_FOUND,
    PB_PARKING_FULL,
    PB_PARKING_EMPTY,
    PB_ERROR                     /**< Error: Generic error*/
} PB_RC;

typedef unsigned long 	uLong;

struct parking_buffer_stats_t {
	uLong n_park_success;
	uLong n_park_failure;
	uLong n_unpark_failure;
};

typedef struct parking_buffer_stats_t 	parking_buffer_stats_t;

typedef struct parking_buffer_t    parking_buffer_t;

/**
* @brief creates parking buffer
*
*/
parking_buffer_t * pb_create(uLong capacity);

/**
* @brief destroy parking buffer
*
*/
void pb_destroy(parking_buffer_t * pb);

/**
* @brief park the car makes entry in the buffer
*
*/
PB_RC pb_park(parking_buffer_t * pb, uLong car_id, uLong * slot);

/**
* @brief unpark the car.. removes entry from the buffer
*
*/
PB_RC pb_unpark(parking_buffer_t * pb, uLong car_id);

/**
* @brief
*
*/
uLong pb_get_free_slots_count(parking_buffer_t * pb );

/**
 * @brief print buffer
 */
void pb_print(parking_buffer_t * pb );

#endif
