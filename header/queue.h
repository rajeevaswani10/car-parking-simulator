#include <stdio.h>
#include <stdlib.h>

typedef struct queue_t queue_t;

//create queue
queue_t * queue_create();

//destroy queue
void queue_destroy( queue_t * q );

// To Enqueue an integer
int queue_enqueue(queue_t * q, void * data);

// To Dequeue an integer.
void * queue_dequeue(queue_t * q);

//size
unsigned int queue_size(queue_t * q);

