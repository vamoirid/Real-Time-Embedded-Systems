/*
 *******************************************************************************
 * Author: Vasileios Amoiridis                                                 *
 * Filename: prod-cons.c                                                       *
 * Date: Mar 19 04:27                                                          *
 *******************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <assert.h>
#include <sys/time.h>

/*
 *******************************************************************************
 * DEFINES                                                                     *
 *******************************************************************************
 */
#define NUM_PROD 1
#define NUM_CONS 1
#define QUEUESIZE 10

/*
 *******************************************************************************
 * queue Node type                                                             *
 *******************************************************************************
 */
//typedef void *(*work)(void *);

typedef struct workFunction {
	void *(*work)(void *);
	void *arg;
} queue_t;

//typedef void *(*queue_t)(void *);

typedef struct {
	queue_t buf[QUEUESIZE];
	long head, tail;
	int full, empty;
	pthread_mutex_t *mut;
	pthread_cond_t *notFull, *notEmpty;
} queue;

/*
 *******************************************************************************
 * Thread Functions' prototypes                                                *
 *******************************************************************************
 */
void *producer(void *args);
void *consumer(void *args);

/*
 *******************************************************************************
 * Functions' prototypes                                                       *
 *******************************************************************************
 */
queue *queueInit ();
void queueAdd    (queue *q, queue_t in);
void queueDel    (queue *q, queue_t *out);
void queueDelete (queue *q);
void *testFunA   (void *);
void *testFunB   (void *);
void *testFunC   (void *);

//queue_t funcArray[3] = {testFunA, testFunB, testFunA};
queue_t funcArray[3];

/*
 *******************************************************************************
 * main()                                                                      *
 *******************************************************************************
 */

int main(int argc, char *argv[])
{
	funcArray[0].work = testFunA;
	funcArray[1].work = testFunB;
	funcArray[2].work = testFunC;

	queue *fifo;
	pthread_t prod, cons[NUM_CONS];
	pthread_attr_t attr;
	long* status;
	int i;

	struct timeval tv;
	suseconds_t usec;	

	fifo = queueInit();
	if (fifo == NULL)
	{
		printf("***ERROR***: An error occured while creating the list.\n");
		exit(1);
	}

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

gettimeofday(&tv, NULL);
usec = tv.tv_usec;

	for(i = 0; i < NUM_PROD; i++)
		pthread_create(&prod, &attr, producer, fifo);
	for (i = 0; i < NUM_CONS; i++)
		pthread_create(&cons[i], &attr, consumer, fifo);
	for(i = 0; i < NUM_PROD; i++)
		pthread_join(prod, NULL);
	for (i = 0; i < NUM_CONS; i++)
		pthread_join(cons[i], NULL);

	/*printf("main(): producer thread exited with %ld status.\n", status);
	printf("main(): consumer thread exited with %ld status.\n", status);*/

	pthread_attr_destroy(&attr);

	return 0;
}
/*
 *******************************************************************************
 * Thread producer                                                             *
 *******************************************************************************
 */
void *producer(void *args)
{
	queue *fifo;
	fifo = (queue *)args;
	int i = 0;

	while (1)
	{
		pthread_mutex_lock(fifo->mut);

		while (fifo->full)
		{
			printf("producer: queue FULL.\n");
			pthread_cond_wait(fifo->notFull, fifo->mut);
		}

		/*struct timeval tv;
		gettimeofday(&tv, NULL);
		fifo->buf[i].arg = &(tv.tv_usec);*/

		queueAdd(fifo, funcArray[i]);
		i++;
		if (i == 3) i = 0;
		pthread_mutex_unlock(fifo->mut);
		//printf("added i = %d\n", i);	
		pthread_cond_signal(fifo->notEmpty);
	}


	pthread_exit(0);
}

/*
 *******************************************************************************
 * Thread consumer                                                             *
 *******************************************************************************
 */
void *consumer(void *args)
{
	queue *fifo;
	fifo = (queue *)args;
	queue_t function2run;
	double *sum, *result;

	while(1)
	{
		pthread_mutex_lock(fifo->mut);

		while(fifo->empty)
		{
			printf("consumer: queue EMPTY.\n");
			pthread_cond_wait(fifo->notEmpty, fifo->mut);
		}

		queueDel(fifo, &function2run);
		//result = (double *)function2run(sum);
		function2run.work(sum);
		pthread_mutex_unlock(fifo->mut);
		//printf("deleted i = %d\n", value);	
		pthread_cond_signal(fifo->notFull);
	}

	pthread_exit(0);
}

/*
 *******************************************************************************
 * queue functions                                                             *
 *******************************************************************************
 */
queue *queueInit()
{
	queue *q;

	q = (queue *)malloc(sizeof(queue));
	if (q == NULL) return(NULL);

	q->head = 0;
	q->tail = 0;
	q->full = 0;
	q->empty = 1;
	q->mut = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(q->mut, NULL);
	q->notFull = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
	pthread_cond_init(q->notFull, NULL);
	q->notEmpty = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
	pthread_cond_init(q->notEmpty, NULL);

	return (q);
}

void queueAdd(queue *q, queue_t in)
{
	q->buf[q->tail] = in;
	q->tail++;

	if (q->tail == QUEUESIZE)
		q->tail = 0;
	if (q->tail == q->head)
		q->full = 1;
	q->empty = 0;

	return;
}

void queueDel(queue *q, queue_t *out)
{
	*out = q->buf[q->head];
	q->head++;

	if (q->head == QUEUESIZE)
		q->head = 0;
	if (q->head == q->tail)
		q->empty = 1;
	q->full = 0;

	return;
}

void queueDelete(queue *q)
{
	pthread_mutex_destroy(q->mut);
	free(q->mut);
	pthread_cond_destroy(q->notFull);
	free(q->notFull);
	pthread_cond_destroy(q->notEmpty);
	free(q->notEmpty);

	free(q);
}

void* testFunA(void *sum)
{
	double i, tot_sum = 0;
	for(i = 0; i < 360; i++) tot_sum += sin(i);
	printf("testFunA(): Hello guys! Did you know that the sum of the sin() from 0 to 360 degrees is %f?\n",tot_sum);
	sum = &tot_sum;

	return (sum);
}

void* testFunB(void *sum)
{
	double i, tot_sum = 0;	
	for(i = 0; i < 360; i++) tot_sum += cos(i);
	printf("testFunB(): Hello guys! Did you know that the sum of the cos() from 0 to 360 degrees is %f?\n",tot_sum);
	sum = &tot_sum;

	return (sum);
}

void* testFunC(void *sum)
{
	double i, tot_sum = 0;
	for(i = 0; i < 360; i++) tot_sum += tan(i);
	printf("testFunC(): Hello guys! Did you know that the sum of the tan() from 0 to 360 degrees is %f?\n",tot_sum);
	sum = &tot_sum;

	return (sum);
}