/*
 *******************************************************************************
 * Author: Vasileios Amoiridis                                                 *
 * Filename: queue.c                                                           *
 * Date: Mar 20 17:38                                                          *
 *******************************************************************************
 */
#ifndef QUEUE_H
#define QUEUE_H

#define QUEUESIZE 15

/*
 *******************************************************************************
 * queue Node arguments' type                                                  *
 *******************************************************************************
 */
typedef struct workFunArgs {
	void *func_args;
	struct timeval tv;
} workFunArgs;

/*
 *******************************************************************************
 * queue Node type                                                             *
 *******************************************************************************
 */
typedef struct workFunction {
	void *(*work)(void *);
	void *args;
} workFunction;

/*
 *******************************************************************************
 * timer type                                                                  *
 *******************************************************************************
 */
typedef struct timer {
	uint16_t period;
	uint32_t tasksToExecute;
	uint32_t startDelay;
	void *(*StartFcn)(void *);
	void *(*StopFcn)();
	void *(*ErrorFcn)();
	void *(*TimerFcn)();
	void *UserData;
} timer;

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

/*
 *******************************************************************************
 * timer type                                                                  *
 *******************************************************************************
 */
typedef struct producerDataType {
	timer t;
	queue *fifo;
} producerDataType;

queue *queueInit ();
void queueAdd    (queue *q, workFunction in);
void queueDel    (queue *q, workFunction *out);
void queueDelete (queue *q);
#endif