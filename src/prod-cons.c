/*
 *******************************************************************************
 * Author: Vasileios Amoiridis                                                 *
 * Filename: prod-cons.c                                                       *
 * Date: Mar 24 04:45                                                          *
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
#define NUM_CONS 4
#define LOOP 2500

/*
 *******************************************************************************
 * Thread Functions' prototypes                                                *
 *******************************************************************************
 */
void *producer(void *args);
void *consumer(void *args);
workFunction *funcArrayInit();
void funcArrayDelete(workFunction *);
int *timeArrayInit();
void timeArrayDelete(int *);

/*
 *******************************************************************************
 * Global Variables                                                            *
 *******************************************************************************
 */
workFunction *funcArray;
int producersFinished = 0;
FILE *fp;
int *timeArray;
int *timeArrayIndex;

/*
 *******************************************************************************
 * main()                                                                      *
 *******************************************************************************
 */
int main(int argc, char *argv[])
{
	
	queue *fifo;
	pthread_t prod[NUM_PROD], cons[NUM_CONS];
	int i, num_prod, num_cons;
	char str[5], filename[30];

	timeArrayIndex = (int *)malloc(sizeof(int));

	funcArray = funcArrayInit();
	timeArray = timeArrayInit();

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
	pthread_cond_broadcast(fifo->notEmpty);

	for (i = 0; i < NUM_CONS; i++)
		pthread_join(cons[i], NULL);

	sprintf(filename, "prod_%d_cons_%d.txt", NUM_PROD, NUM_CONS);

	fp = fopen(filename, "w");
	for(i = 0; i < NUM_PROD * LOOP; i ++)
	{
		sprintf(str, "%d\n", *(timeArray+i));
		fputs(str, fp);
	}
	fclose(fp);

	free(timeArrayIndex);
	timeArrayDelete(timeArray);
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
	workFunArgs *timeNargsT;                                     // NEW
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

		timeNargsT = (workFunArgs *)malloc(sizeof(workFunArgs));
		timeNargsT->func_args = random_corner;

		gettimeofday(&timeNargsT->tv, NULL);
		suseconds_t usec_new = timeNargsT->tv.tv_usec;

		funcArray[r].args = timeNargsT;

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
	workFunArgs *timeNargsR;
	double *retVal;
	struct timeval time_arrived;
	int i = 0;

	while(1)
	{
		if (fifo->empty && producersFinished) 
		{
			//printf("---------------------------------------- 1\n");
			pthread_cond_broadcast(fifo->notEmpty);
			pthread_exit(0);
		}

		pthread_mutex_lock(fifo->mut);

		while (fifo->empty)
		{
			if (producersFinished) 
			{
				//printf("---------------------------------------- 2\n");
				pthread_cond_broadcast(fifo->notEmpty);
				pthread_exit(0);
			}
			//printf("consumer: queue EMPTY.\n");
			pthread_cond_wait(fifo->notEmpty, fifo->mut);
			if (producersFinished)
			{
				//printf("---------------------------------------- 3\n");
				pthread_mutex_unlock(fifo->mut);
				pthread_cond_broadcast(fifo->notEmpty);
				pthread_exit(0);
			} 
		}

		queueDel(fifo, &receivedWorkFunc);
		gettimeofday(&time_arrived, NULL);

		timeNargsR = (workFunArgs *)receivedWorkFunc.args;
		//printf("%ld\n", time_arrived.tv_usec - timeNargsR->tv.tv_usec);

		timeArray[*timeArrayIndex] = (int)(time_arrived.tv_usec - timeNargsR->tv.tv_usec);
		(*timeArrayIndex)++;

		pthread_mutex_unlock(fifo->mut);
		pthread_cond_signal(fifo->notFull);
		
		//printf("Time needed = %ld\n", time_arrived.tv_usec - timeNargsR->tv.tv_usec);
		retVal = receivedWorkFunc.work(timeNargsR->func_args);
		//printf("retVal = %.3f\n", *retVal);

		free(timeNargsR);
		free(retVal);
	}
}

workFunction* funcArrayInit()
{
	funcArray = (workFunction *) malloc(4 * sizeof(workFunction));

	funcArray[0].work = testFunction0;
	funcArray[0].args = NULL;
	funcArray[1].work = testFunction1;
	funcArray[1].args = NULL;
	funcArray[2].work = testFunction2;
	funcArray[2].args = NULL;
	funcArray[3].work = testFunction3;
	funcArray[3].args = NULL;

	return funcArray;
}

void funcArrayDelete(workFunction *funcArray)
{
	free(funcArray);
}

int *timeArrayInit()
{
	int *timeArray = (int *)malloc(NUM_PROD * LOOP * sizeof(int));

	return timeArray;
}

void timeArrayDelete(int *timeArray)
{
	free(timeArray);
}