/*
 *******************************************************************************
 * Author: Vasileios Amoiridis                                                 *
 * Filename: prod-cons.c                                                       *
 * Date: Mar 17 01:00                                                          *
 *******************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthreads.h>
#include <semaphore.h>

/*
 *******************************************************************************
 * DEFINES                                                                     *
 *******************************************************************************
 */
#define NUM_PROD 10
#define NUM_CONS 10
#define QUEUESIZE 10

typedef struct {
	int buf[QUEUESIZE];
	long head, tail;
	int full, empty;
	pthread_mutex_t *mut;
	pthread_cond_t *notFull, *notEmpty;
	sem_t *sem;
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
queue queueInit();
void queueAdd(queue *q, int in);
void queueDel(queue *q, int *out);
void queueDelete(queue *q);

queue queueInit()
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
	pthread_cond_init(q->notFull, q->mut);
	q->notEmpty = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
	pthread_cond_init(q->notEmpty, q->mut);
	q->sem = (sem *) malloc(sizeof(sem));
	sem_init(q->sem, 0, 0);

	return (q);
}

void queueAdd(queue *q, int in)
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

void queueDel(queue *q, int *out)
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
	sem_destroy(q->sem);
	free(q->sem);
	free(q);
}