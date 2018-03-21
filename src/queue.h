#include<stdio.h>
#include<stdlib.h>

typedef struct queue_t queue_t;

//create queue
queue_t * queue_create();

//destroy queue
void queue_destroy( queue_t * q );

// To Enqueue an integer
int queue_enqueue(queue_t * q, int data);

// To Dequeue an integer.
int queue_dequeue(queue_t * q, int * data);

//size
unsigned int queue_size(queue_t * q);

//print
void queue_print(queue_t * q);


