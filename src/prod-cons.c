/*
 *******************************************************************************
 * Author: Vasileios Amoiridis                                                 *
 * Filename: prod-cons.c                                                       *
 * Date: Mar 20 04:52                                                          *
 *******************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

/*
 *******************************************************************************
 * DEFINES                                                                     *
 *******************************************************************************
 */
#define NUM_PROD 4
#define NUM_CONS 8
#define QUEUESIZE 10
#define LOOP 10

/*
 *******************************************************************************
 * queue Node type                                                             *
 *******************************************************************************
 */

typedef struct workFunction {
	void *(*work)(void *);
	void *arg;
} workFunction;

typedef struct {
	workFunction buf[QUEUESIZE];
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
void queueAdd    (queue *q, workFunction in);
void queueDel    (queue *q, workFunction *out);
void queueDelete (queue *q);
void *testFunction0   (void *);
void *testFunction1   (void *);
void *testFunction2   (void *);
void *testFunction3   (void *);
void *testFunction4   (void *);
void *testFunction5   (void *);
void *testFunction6   (void *);
void *testFunction7   (void *);
void *testFunction8   (void *);
void *testFunction9   (void *);

workFunction funcArray[10];

/*
 *******************************************************************************
 * main()                                                                      *
 *******************************************************************************
 */

int main(int argc, char *argv[])
{
	//funcArray = (workFunction *) malloc(10 * sizeof(workFunction));
	funcArray[0].work = testFunction0;
	funcArray[1].work = testFunction1;
	funcArray[2].work = testFunction2;
	funcArray[3].work = testFunction3;
	funcArray[4].work = testFunction4;
	funcArray[5].work = testFunction5;
	funcArray[6].work = testFunction6;
	funcArray[7].work = testFunction7;
	funcArray[8].work = testFunction8;
	funcArray[9].work = testFunction9;
	
	queue *fifo;
	pthread_t prod[NUM_PROD], cons[NUM_CONS];
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

	for(i = 0; i < NUM_PROD; i++)
		pthread_create(&prod[i], NULL, producer, fifo);
	for (i = 0; i < NUM_CONS; i++)
		pthread_create(&cons[i], NULL, consumer, fifo);
	for(i = 0; i < NUM_PROD; i++)
		pthread_join(prod[i], NULL);
	for (i = 0; i < NUM_CONS; i++)
		pthread_join(cons[i], NULL);

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
	long long address = (long long)&i;
	srand(address);

	for(i = 0; i < LOOP; i++)
	//while (1)
	{
		pthread_mutex_lock(fifo->mut);

		while (fifo->full)
		{
			//printf("producer: queue FULL.\n");
			pthread_cond_wait(fifo->notFull, fifo->mut);
		}

		int r = rand() % 10; //random int between [0,9]
		printf("%d\n", r);
		queueAdd(fifo, funcArray[r]);
		pthread_mutex_unlock(fifo->mut);
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
	workFunction receivedWorkFunc;
	double *sum;

	while(1)
	{
		pthread_mutex_lock(fifo->mut);

		while (fifo->empty)
		{
			//printf("consumer: queue EMPTY.\n");
			pthread_cond_wait(fifo->notEmpty, fifo->mut);
		}

		queueDel(fifo, &receivedWorkFunc);
		pthread_mutex_unlock(fifo->mut);
		pthread_cond_signal(fifo->notFull);

		receivedWorkFunc.work(sum); //execute after signal!!!
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

void queueAdd(queue *q, workFunction in)
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

void queueDel(queue *q, workFunction *out)
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

void* testFunction0(void *sum)
{
	double i, tot_sum = 0;	
	for(i = 0; i < 360; i++) tot_sum += sin(i);
	sum = &tot_sum;
	printf("testFunction0\n");

	return (sum);
}

void* testFunction1(void *sum)
{
	double i, tot_sum = 0;
	for(i = 0; i < 360; i++) tot_sum += cos(i);
	sum = &tot_sum;
	printf("testFunction1\n");

	return (sum);
}

void* testFunction2(void *sum)
{
	double i, tot_sum = 0;	
	for(i = 0; i < 360; i++) tot_sum += tan(i);
	sum = &tot_sum;
	printf("testFunction2\n");

	return (sum);
}

void* testFunction3(void *sum)
{
	double i, tot_sum = 0;
	for(i = 0; i < 360; i++) tot_sum += sin(i)*cos(i);
	sum = &tot_sum;
	printf("testFunction3\n");

	return (sum);
}

void* testFunction4(void *sum)
{
	double i, tot_sum = 0;
	for(i = 0; i < 360; i++) tot_sum += sin(i)*tan(i);
	sum = &tot_sum;
	printf("testFunction4\n");

	return (sum);
}

void* testFunction5(void *sum)
{
	double i, tot_sum = 0;	
	for(i = 0; i < 360; i++) tot_sum += cos(i)*tan(i);
	sum = &tot_sum;

	return (sum);
}

void* testFunction6(void *sum)
{
	double i, tot_sum = 0;
	for(i = 0; i < 360; i++) tot_sum += sin(i)*sin(i);
	sum = &tot_sum;
	printf("testFunction6\n");


	return (sum);
}

void* testFunction7(void *sum)
{
	double i, tot_sum = 0;
	for(i = 0; i < 360; i++) tot_sum += cos(i)*cos(i);
	sum = &tot_sum;
	printf("testFunction7\n");


	return (sum);
}

void* testFunction8(void *sum)
{
	double i, tot_sum = 0;	
	for(i = 0; i < 360; i++) tot_sum += tan(i)*tan(i);
	sum = &tot_sum;
	printf("testFunction8\n");

	return (sum);
}

void* testFunction9(void *sum)
{
	double i, tot_sum = 0;
	for(i = 0; i < 360; i++) tot_sum += sin(i)*cos(i)*tan(i);
	sum = &tot_sum;
	printf("testFunction9\n");

	return (sum);
}