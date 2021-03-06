/*
 *******************************************************************************
 * Author: Vasileios Amoiridis                                                 *
 * Filename: queue.c                                                           *
 * Date: Mar 20 17:38                                                          *
 *******************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include "../inc/queue.h"

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
