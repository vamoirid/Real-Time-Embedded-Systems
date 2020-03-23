/*
 *******************************************************************************
 * Author: Vasileios Amoiridis                                                 *
 * Filename: prod-cons.c                                                       *
 * Date: Mar 23 02:25                                                          *
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
#define LOOP 100000

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
	pthread_cond_signal(fifo->notEmpty);
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
	int i, r;
	double *random_corner;
	long long address = (long long)&i;
	srand(address);

	for(i = 0; i < LOOP; i++)
	{
		pthread_mutex_lock(fifo->mut);

		while (fifo->full)
		{
			//printf("producer: queue FULL.\n");
			pthread_cond_wait(fifo->notFull, fifo->mut);
		}

		r = rand() % 4; //random int between [0,3]
		random_corner = (double *)malloc(sizeof(double));
		*random_corner = rand() % 360;
		funcArray[r].timeNargs.args = random_corner;

		gettimeofday(&funcArray[r].timeNargs.tv, NULL);
		suseconds_t usec = funcArray[r].timeNargs.tv.tv_usec;

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

		pthread_mutex_unlock(fifo->mut);
		pthread_cond_signal(fifo->notFull);
		printf("Time needed = %ld\n", time_arrived.tv_usec - receivedWorkFunc.timeNargs.tv.tv_usec);

		retVal = receivedWorkFunc.work(receivedWorkFunc.timeNargs.args);
		//printf("retVal = %f\n", *retVal);
		free(retVal);
	}

	pthread_exit(0);
}

workFunction* funcArrayInit()
{
	funcArray = (workFunction *) malloc(4 * sizeof(workFunction));

	funcArray[0].work = testFunction0;
	//funcArray[0].timeNargs.args = NULL;
	funcArray[1].work = testFunction1;
	//funcArray[1].timeNargs.args = NULL;
	funcArray[2].work = testFunction2;
	//funcArray[2].timeNargs.args = NULL;
	funcArray[3].work = testFunction3;
	//funcArray[3].timeNargs.args = NULL;

	return funcArray;
}

void funcArrayDelete(workFunction *funcArray)
{
	free(funcArray);
}