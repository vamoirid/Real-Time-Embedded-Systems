/*
 *******************************************************************************
 * Author: Vasileios Amoiridis                                                 *
 * Filename: queue.c                                                           *
 * Date: Mar 20 17:38                                                          *
 *******************************************************************************
 */
#ifndef QUEUE_H
#define QUEUE_H

#define QUEUESIZE 10

/*
 *******************************************************************************
 * queue Node arguments' type                                                  *
 *******************************************************************************
 */
typedef struct workFunArgs {
	void *args;
	struct timeval tv;
} workFunArgs;

/*
 *******************************************************************************
 * queue Node type                                                             *
 *******************************************************************************
 */
typedef struct workFunction {
	void *(*work)(void *);
	workFunArgs timeNargs;
} workFunction;

/*
 *******************************************************************************
 * queue variables                                                             *
 *******************************************************************************
 */
typedef struct {
	workFunction buf[QUEUESIZE];
	long head, tail;
	int full, empty;
	pthread_mutex_t *mut;
	pthread_cond_t *notFull, *notEmpty;
} queue;

queue *queueInit ();
void queueAdd    (queue *q, workFunction in);
void queueDel    (queue *q, workFunction *out);
void queueDelete (queue *q);
#endif