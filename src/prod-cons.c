/*
 *******************************************************************************
 * Author: Vasileios Amoiridis                                                 *
 * Filename: prod-cons.c                                                       *
 * Date: Mar 21 04:00                                                          *
 *******************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>
#include "/home/vamoirid/Desktop/git/Real-Time-Embedded-Systems/inc/queue.h"
#include "/home/vamoirid/Desktop/git/Real-Time-Embedded-Systems/inc/functionDataBase.h"

/*
 *******************************************************************************
 * DEFINES                                                                     *
 *******************************************************************************
 */
#define NUM_PROD 4
#define NUM_CONS 8
#define LOOP 10

/*
 *******************************************************************************
 * Thread Functions' prototypes                                                *
 *******************************************************************************
 */
void *producer(void *args);
void *consumer(void *args);
workFunction* funcArrayInit();

/*
 *******************************************************************************
 * Global Variables                                                            *
 *******************************************************************************
 */
workFunction *funcArray;

/*
 *******************************************************************************
 * main()                                                                      *
 *******************************************************************************
 */
int main(int argc, char *argv[])
{
	
	queue *fifo;
	pthread_t prod[NUM_PROD], cons[NUM_CONS];
	long* status;
	int i;

	funcArray = funcArrayInit();

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
	srand(time(NULL));

	for(i = 0; i < LOOP; i++)
	//while (1)
	{
		pthread_mutex_lock(fifo->mut);

		while (fifo->full)
		{
			//printf("producer: queue FULL.\n");
			pthread_cond_wait(fifo->notFull, fifo->mut);
		}

		int r = rand() % 3; //random int between [0,9]
		double *random_corner = (double *)malloc(sizeof(double));
		*random_corner = rand() % 360;

		funcArray[r].args = random_corner;
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
	double *retVal;

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

		retVal = receivedWorkFunc.work(receivedWorkFunc.args); //execute after signal!!!
		free(retVal);
	}
	pthread_exit(0);
}

workFunction* funcArrayInit()
{
	funcArray = (workFunction *) malloc(10 * sizeof(workFunction));

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

	return funcArray;
}