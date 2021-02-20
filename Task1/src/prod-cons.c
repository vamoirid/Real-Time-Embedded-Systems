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
#include "../inc/queue.h"
#include "../inc/functionDataBase.h"

/*
 *******************************************************************************
 * DEFINES                                                                     *
 *******************************************************************************
 */
#define NUM_PROD 3
#define NUM_CONS 6
#define LOOP 5

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
	int i, times_sum = 0;
	char str[5], filename[30], meanfile[30], meanname[30];
	double mean_value = 0.0;

	timeArrayIndex = (int *)malloc(sizeof(int));
	*timeArrayIndex = 0; // valgrind is ok now

	funcArray = funcArrayInit();
	timeArray = timeArrayInit();

	fifo = queueInit();
	if (fifo == NULL)
	{
		printf("***ERROR***: An error occured while creating the QUEUE.\n");
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
	while(!fifo->empty);
	pthread_cond_broadcast(fifo->notEmpty);

	for (i = 0; i < NUM_CONS; i++)
		pthread_join(cons[i], NULL);

	// Generate a file with the calculated elapsed times.
	sprintf(filename, "prod%d_cons%d.txt", NUM_PROD, NUM_CONS);

	fp = fopen(filename, "w");
	for(i = 0; i < NUM_PROD * LOOP; i ++)
	{
		if(*(timeArray+i) > 500) *(timeArray+i) = 10;
		sprintf(str, "%d\n", *(timeArray+i));
		fputs(str, fp);
		times_sum += *(timeArray+i);
	}
	fclose(fp);

	// Generate file with the mean_values IF multiple loops are used.
	mean_value = (double) times_sum / (NUM_PROD * LOOP);
	times_sum = 0;
	sprintf(meanfile, "MEAN_VALUES_prod%d.txt", NUM_PROD);
	sprintf(meanname, "mean_%d_%d = %.3f\n", NUM_PROD, NUM_CONS, mean_value);
	
	fp = fopen(meanfile, "a");
	fputs(meanname, fp);
	fclose(fp);

	free(timeArrayIndex);       // Allocated in main.
	timeArrayDelete(timeArray); // Allocated in main.
	queueDelete(fifo);          // Allocated in main.
	funcArrayDelete(funcArray); // Allocated in main.

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
	workFunArgs *timeNargsT;
	srand(address); // Every thread has different stack so &i is unique.

	for(i = 0; i < LOOP; i++)
	{
		pthread_mutex_lock(fifo->mut);

		while (fifo->full)
		{
			//printf("producer: queue FULL.\n");
			pthread_cond_wait(fifo->notFull, fifo->mut);
		}

		// Assign random values for function and corner
		r = 3;//rand() % 4; //random int between [0,3]
		random_corner = (double *)malloc(sizeof(double));
		*random_corner = rand() % 360;
		// Allocate space and assign corner value
		timeNargsT = (workFunArgs *)malloc(sizeof(workFunArgs));
		timeNargsT->func_args = random_corner;
		gettimeofday(&timeNargsT->tv, NULL);
		// Assign the arguments' adress to the function and queueAdd.
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
	long elapsed_time = 0;

	while(1)
	{
		// If you we running a function during cond_broadcast exit here.
		if (fifo->empty && producersFinished) 
		{
			pthread_cond_broadcast(fifo->notEmpty);
			pthread_exit(0);
		}

		pthread_mutex_lock(fifo->mut);

		while (fifo->empty)
		{
			// If you were stuch in mutex_lock() during cond_broadcast 
			// exit here.
			if (producersFinished) 
			{
				pthread_mutex_unlock(fifo->mut);
				pthread_cond_broadcast(fifo->notEmpty);
				pthread_exit(0);
			}

			//printf("consumer: queue EMPTY.\n");
			pthread_cond_wait(fifo->notEmpty, fifo->mut);
			// If you were stuck in pthread_cond_wait during cond_broadcast
			// eixt here.
			if (producersFinished)
			{
				pthread_mutex_unlock(fifo->mut);
				pthread_cond_broadcast(fifo->notEmpty);
				pthread_exit(0);
			} 
		}

		queueDel(fifo, &receivedWorkFunc);
		gettimeofday(&time_arrived, NULL);
		// Take arguments and calculate time passed.
		timeNargsR = (workFunArgs *)receivedWorkFunc.args;
		elapsed_time = (time_arrived.tv_sec - timeNargsR->tv.tv_sec)*1000000 
						+ ((time_arrived.tv_usec - timeNargsR->tv.tv_usec));
		timeArray[*timeArrayIndex] = elapsed_time;
		(*timeArrayIndex)++;

		pthread_mutex_unlock(fifo->mut);
		pthread_cond_signal(fifo->notFull);
		
		retVal = receivedWorkFunc.work(timeNargsR->func_args);

		free(timeNargsR); //Allocated in producer.
		free(retVal);     //Allocated in running function.
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

	for(int i = 0; i < NUM_PROD * LOOP; i++)
		*(timeArray+i) = 0; // valgrind is OK now

	return timeArray;
}

void timeArrayDelete(int *timeArray)
{
	free(timeArray);
}
