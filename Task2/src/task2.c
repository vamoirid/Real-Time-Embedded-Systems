/*
 *******************************************************************************
 * Author: Vasileios Amoiridis                                                 *
 * Filename: task2.c                                                           *
 * Date: Feb 20 13:36                                                          *
 *******************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
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
#define MAX_NUM_PROD 3
#define NUM_CONS 6
#define LOOP 10000
#define NUM_JOBS 5000

/*
 *******************************************************************************
 * Thread Functions' prototypes                                                *
 *******************************************************************************
 */
void *producer(void *args);
void *consumer(void *args);
void timerInit(timer *t, uint16_t period, uint32_t tasksToExecute, uint32_t startDelay,
               void *(*startTimerFcn)(void*), void *(*stopTimerFcn)(), void *(*errorTimerFcn)(), 
               void *(*TimerFcn)(), void *UserData);
void *startTimerFcn(void *);
void start(timer *);
void startat(timer *t, int year, int month, int day, int hour, int min, int sec);
void *stopTimerFcn();
void *errorTimerFcn();

/*
 *******************************************************************************
 * Global Variables                                                            *
 *******************************************************************************
 */
workFunction *funcArray;
int producersFinished = 0;
FILE *fp_timeInQueue, *fp_timedriftT1, *fp_timedriftT2, *fp_timedriftT3;
timer *t1,*t2,*t3;
uint8_t numOfProducers = 0;
pthread_t prod[MAX_NUM_PROD], cons[NUM_CONS];
queue *globalFIFO;

/*
 *******************************************************************************
 * main()                                                                      *
 *******************************************************************************
 */
int main(int argc, char *argv[])
{
	int i, times_sum = 0;
	char str[9], *filename;
	double *random_corner;
	int corner1 = 30, corner2 = 45, corner3 = 60;

	t1 = (timer *)malloc(sizeof(timer));
	t2 = (timer *)malloc(sizeof(timer));
	t3 = (timer *)malloc(sizeof(timer));

	timerInit(t1, 1000, 36, 0, startTimerFcn, stopTimerFcn, errorTimerFcn, timerFcn0, &corner1);
	timerInit(t2, 100, 360, 0, startTimerFcn, stopTimerFcn, errorTimerFcn, timerFcn1, &corner2);
	timerInit(t3, 10, 3600, 0, startTimerFcn, stopTimerFcn, errorTimerFcn, timerFcn2, &corner3);

	globalFIFO = queueInit();
	if (globalFIFO == NULL)
	{
		printf("***ERROR***: An error occured while creating the QUEUE.\n");
		exit(1);
	}

	filename = "TimeInQueue.csv";
	fp_timeInQueue = fopen(filename, "w");
	printf("Here 1\n");
	start(t1); //startat(t1, 2021, 2, 20, 2, 13, 0);
	filename = "Timedrift_T1.csv";
	fp_timedriftT1 = fopen(filename, "w");
	start(t2);
	filename = "Timerift_T2.csv";
	fp_timedriftT2 = fopen(filename, "w");
	start(t3);
	filename = "Timedrift_T3.csv";
	fp_timedriftT3 = fopen(filename, "w");

	for (i = 0; i < NUM_CONS; i++)
		pthread_create(&cons[i], NULL, consumer, globalFIFO);

	for(int i = 0; i < numOfProducers; i++)
	pthread_join(prod[i], NULL);

	producersFinished = 1;
	pthread_cond_broadcast(globalFIFO->notEmpty);
	while(!globalFIFO->empty);
	pthread_cond_broadcast(globalFIFO->notEmpty);

	for(int i = 0; i < NUM_CONS; i++)
		pthread_join(cons[i], NULL);

	fclose(fp_timeInQueue);
	fclose(fp_timedriftT1);
	fclose(fp_timedriftT2);
	fclose(fp_timedriftT3);

	free(t1);                                 // Allocated in main
	free(t2);                                 // Allocated in main
	free(t3);                                 // Allocated in main
	queueDelete(globalFIFO);                  // Allocated in main

	return 0;
}

/*
 *******************************************************************************
 * Thread producer                                                             *
 *******************************************************************************
 */
void *producer(void *args)
{
	producerDataType *pdt;
	int period, tasksToExecute, timedrift, totalTimedrift;
	queue *fifo;
	long long timePassed, lastPeriodTime, timeNow, sleepTime;
	char str[9];
	struct timeval timestamp;
	double *random_corner; 
	workFunction task;
	workFunArgs *timeNargsT;

	pdt = (producerDataType *)args;
	period = pdt->t.period;
	tasksToExecute = pdt->t.tasksToExecute;
	fifo = pdt->fifo;
	sleepTime = period * 1000;
	task.work = pdt->t.TimerFcn;
	int *data = (int *)pdt->t.UserData;
	void *startArgs, *stopArgs, *errorArgs;
	pdt->t.StartFcn(startArgs);

	usleep(pdt->t.startDelay*1000000); //Start up delay

	for(int i = 0; i < tasksToExecute; i++)
	{
		pthread_mutex_lock(fifo->mut);
		while (fifo->full)
		{
			//printf("producer: queue FULL.\n");
			pthread_cond_wait(fifo->notFull, fifo->mut);
		}

		// Allocate space and assign corner value
		timeNargsT = (workFunArgs *)malloc(sizeof(workFunArgs));
		timeNargsT->func_args = pdt->t.UserData;
		gettimeofday(&timeNargsT->tv, NULL);
		// Assign the arguments' address to the function and queueAdd.
		task.args = timeNargsT;
		queueAdd(fifo, task);
		gettimeofday(&timestamp, NULL);
		timeNow = (long long)timestamp.tv_sec*1000000 + (long long)timestamp.tv_usec;
		
		pthread_mutex_unlock(fifo->mut);
		pthread_cond_signal(fifo->notEmpty);

		if (i > 0) //not in the first loop
		{
			timePassed = timeNow - lastPeriodTime;
			timedrift = timePassed - period*1000;
			sleepTime = sleepTime - timedrift;
			//printf("timedrift is: %4d, timePassed is: %7lld sleepTime = %7lld\n", timedrift, timePassed, sleepTime);
			if (period == 10) 
			{
				sprintf(str, "%d\n", timedrift);
				fputs(str, fp_timedriftT3);
			}
			else if (period == 100)
			{
				sprintf(str, "%d\n", timedrift);
				fputs(str, fp_timedriftT2);
			}
			else 
			{
				sprintf(str, "%d\n", timedrift);
				fputs(str, fp_timedriftT1);
			}
		}

		lastPeriodTime = timeNow;

		if(sleepTime < 0) {sleepTime = 0;}

		usleep(sleepTime);

	}

	pdt->t.StopFcn(stopArgs);
	free(pdt);
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
	char str[9];

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

		sprintf(str, "%ld\n", elapsed_time);
		fputs(str, fp_timeInQueue);
		printf("Total time in queue: %ld us\n", elapsed_time);

		pthread_mutex_unlock(fifo->mut);
		pthread_cond_signal(fifo->notFull);
		
		retVal = receivedWorkFunc.work(timeNargsR->func_args);

		free(timeNargsR); //Allocated in producer.
		free(retVal);     //Allocated in running function.
	}
}

void *startTimerFcn(void *args)
{
	printf("Hello from StartFcn!\n");
}

void start(timer *t)
{
	producerDataType *pdt;
	pdt = (producerDataType *)malloc(sizeof(producerDataType));
	pdt->t = *t;
	pdt->fifo = globalFIFO;
	pthread_create(&prod[numOfProducers], NULL, producer, pdt);
	numOfProducers++;
}

void startat(timer *t, int year, int month, int day, int hour, int min, int sec)
{
	printf("Here 2\n");
	int delay;
	int delay_secs;
	time_t now;
	struct tm calendar_time;
	now = time(NULL);
	printf("Here 3\n");
	calendar_time = *localtime(&now);
	printf("Here 4\n");
	calendar_time.tm_year = year - 1900;
	calendar_time.tm_mon = month - 1;
	calendar_time.tm_mday = day;
	calendar_time.tm_hour = hour;
	calendar_time.tm_min = min;
	calendar_time.tm_sec = sec;
	delay_secs = difftime(mktime(&calendar_time), now);
	printf("Here 5 %d \n", delay_secs);
	delay = (int)delay_secs;
	printf("Here 6 %d \n", delay);

	printf("Total wait time of %d seconds.\n", delay_secs);

	if (delay > 0)
	{
		printf("Total wait time of %d seconds.\n", delay_secs);
		sleep(delay);
		start(t);
	}
	else
	{
		printf("Invalid time value. Can't travel to the past. Yet.\n");
		return;
	}
	return;
}

void *stopTimerFcn()
{
	printf("Bye from StopFcn!\n");
}

void *errorTimerFcn()
{
	printf("Oops from ErrorFcn!\n");
}

void timerInit(timer *t, uint16_t period, uint32_t tasksToExecute, uint32_t startDelay,
               void *(*startTimerFcn)(void*), void *(*stopTimerFcn)(), void *(*errorTimerFcn)(), 
               void *(*TimerFcn)(), void *UserData)
{
	t->period = period;
	t->tasksToExecute = tasksToExecute;
	t->startDelay = startDelay;
	t->StartFcn = startTimerFcn;
	t->StopFcn = stopTimerFcn;
	t->ErrorFcn = errorTimerFcn;
	t->TimerFcn = TimerFcn;
	t->UserData = UserData;
}