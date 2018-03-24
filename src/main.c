/*********
*  @brief Car parking simulator
* 
*
**************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <header/parking_buffer.h>
#include <header/list.h>

static
volatile int keepRunning = 1;

parking_buffer_t* pb ;
llist_t * departures;

uLong n_invalet;
pthread_t * in_valet_t;

uLong n_outvalet;
pthread_t * out_valet_t;

pthread_t monitor_t;
pthread_barrier_t mybarrier;

static
long generate_car_id (long seed)
{
//	int retval;
//	time_t t;
//	/* Intializes random number generator */
//	srand(((unsigned) time(&t)) + seed);

//	return rand() % 99999 + 1000;
	static uLong  n = 1000;
	return n++;
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

void sigHandler(int dummy) {
	//printf("SIG HANDLER in action *********************\n");
	keepRunning = 0;
}

static
void join_threads(){
	int rc,i;

	for(i=0;i<n_invalet;i++){
		rc = pthread_join(in_valet_t[i], NULL);
	}

	for(i=0;i<n_outvalet;i++){
		rc = pthread_join(out_valet_t[i], NULL);
	}

	rc = pthread_join(monitor_t, NULL);
}

static
void * in_valet (void * arg){
	static unsigned int id = 0;
	uLong tid = pthread_self();
	int rc;

	unsigned int myid = ++id;

	printf("Car-in-valet: %u with thread_id: %lu is initialized\n",myid,tid);
	pthread_barrier_wait(&mybarrier);
	while( keepRunning )
	{
		uLong car_id = generate_car_id(id);
		uLong slot;
		rc = pb_park(pb,car_id, &slot);
		if(rc == PB_SUCCESS) {
			printf("Car-in-valet %u - INFO - Car id: %lu in slot %lu\n", myid, car_id, slot);
			llist_insert_last(departures, (void *)car_id);
		}
		else
		{
			printf("Car-in-valet %u - ERROR - Car id: %lu fail to park. retcode: %d\n", myid, car_id, rc);
		}

		fflush(stdout);
		randomSleep(1000000 , tid);
	}
	printf("Car-in-valet: %u with thread_id: %lu is shutting down\n",myid,tid);

}

static
void * out_valet (void * arg)
{
	static unsigned int id = 0;
	unsigned long tid = pthread_self();
	int rc;

	unsigned int myid = ++id;
	printf("Car-out-valet: %u with thread_id: %lu is initialized\n",myid,tid);
	pthread_barrier_wait(&mybarrier);
	while(keepRunning)
	{
		void * tmp = llist_pop_element_at_random(departures);
		if(tmp != NULL){
			uLong car_id = (uLong)tmp;
			rc = pb_unpark(pb,car_id);
			if(rc == PB_SUCCESS) {
				printf("Car-out-valet %u - INFO - Car id %lu has left\n", myid, car_id);
			}
			else
				printf("Car-out-valet %u - ERROR - Car id %lu fail to un-park. retcode %d\n", myid, car_id, rc);
		}

		fflush(stdout);
		randomSleep(1000000 , tid);
	}
	printf("Car-out-valet: %u with thread_id: %lu is shutting down\n",myid,tid);
}

static
void * monitor (void * arg)
{
	unsigned long tid = pthread_self();
	printf("Monitor thread with thread_id: %lu is initialized\n", tid);
	pthread_barrier_wait(&mybarrier);
	while(keepRunning){
		pb_print(pb);
		fflush(stdout);
		sleep(1);
	}
	printf("Monitor thread with thread_id: %lu is shutting down\n", tid);
}


int main(int argc, char * argv[]){

	int rc;
	int i;

	if(argc != 4) //including file name
	{
		printf("error : insufficient parameters\n usage for eg.: <program>  10 4 1 \n\n");
		return -1;
	}

	char * ptr;

	uLong capacity = strtol(argv[1], &ptr, 10);
	n_invalet = strtol(argv[2], &ptr, 10);
	n_outvalet = strtol(argv[3], &ptr, 10);

	/* validation not working..
	//do basic validation..
	if(capacity <= 0){
		printf("error: capacity shall be greater than 0\n");
		return -1;
	}

	if(n_invalet <= 0 || n_outvalet <= 0){
		printf("error: number of invalet threads or outvalet threads shall be greater than 0\n");
		return -1;
	}

*/
	signal(SIGINT, sigHandler);

	pthread_barrier_init(&mybarrier, NULL, n_invalet + n_outvalet + 2);

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

	pthread_barrier_wait(&mybarrier);

	join_threads(); //else main thread will go out.

	pthread_barrier_destroy(&mybarrier);
	llist_destroy(departures);

	pb_destroy(pb);
	printf("\n****************End of main thread***********\n");
}

