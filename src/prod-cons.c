/*
 *******************************************************************************
 * Author: Vasileios Amoiridis                                                 *
 * Filename: prod-cons.c                                                       *
 * Date: Mar 22 04:46                                                          *
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
#define NUM_PROD 8
#define NUM_CONS 8
#define LOOP 100

/*
 *******************************************************************************
 * Thread Functions' prototypes                                                *
 *******************************************************************************
 */
void *producer(void *args);
void *consumer(void *args);
workFunction *funcArrayInit();
void funcArrayDelete(workFunction *);


/*
 *******************************************************************************
 * Global Variables                                                            *
 *******************************************************************************
 */
workFunction *funcArray;
int producersFinished = 0;

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

	producersFinished = 1;

	pthread_mutex_lock(fifo->mut);
	pthread_cond_broadcast(fifo->notEmpty);
	pthread_mutex_unlock(fifo->mut);

	for (i = 0; i < NUM_CONS; i++)
		pthread_join(cons[i], NULL);

	queueDelete(fifo);
	funcArrayDelete(funcArray);

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

	srand(time(NULL));

	for(i = 0; i < LOOP; i++)
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
		funcArray[r].timeNargs.args = &random_corner;

		gettimeofday(&funcArray[r].timeNargs.tv, NULL);
		suseconds_t usec = funcArray[r].timeNargs.tv.tv_usec;
		
		queueAdd(fifo, funcArray[r]);
		pthread_mutex_unlock(fifo->mut);
		pthread_cond_signal(fifo->notEmpty);

		free(random_corner);
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
	struct timeval time_arrived;

	while(1)
	{
		if (fifo->empty && producersFinished) pthread_exit(0);

		pthread_mutex_lock(fifo->mut);

		while (fifo->empty)
		{
			//printf("consumer: queue EMPTY.\n");
			pthread_cond_wait(fifo->notEmpty, fifo->mut);
			if (producersFinished)
			{
				pthread_mutex_unlock(fifo->mut);
				pthread_exit(0);
			} 
		}

		queueDel(fifo, &receivedWorkFunc);

		gettimeofday(&time_arrived, NULL);
		//printf("Time needed = %ld\n", time_arrived.tv_usec - receivedWorkFunc.timeNargs.tv.tv_usec);
		pthread_mutex_unlock(fifo->mut);
		pthread_cond_signal(fifo->notFull);

		retVal = receivedWorkFunc.work(receivedWorkFunc.timeNargs.args);

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

void funcArrayDelete(workFunction *funcArray)
{
	free(funcArray);
}