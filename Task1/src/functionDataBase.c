/*
 *******************************************************************************
 * Author: Vasileios Amoiridis                                                 *
 * Filename: functionDataBase.c                                                *
 * Date: Mar 22 23:42                                                          *
 *******************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../inc/functionDataBase.h"

void *testFunction0(void *args)
{
	double *sum = (double *)malloc(sizeof(double));
	if(!sum) return NULL;
	*sum = sin(*(double *)args * PI / 180.0);
	free(args); // Allocated in producer.

	return (sum);
}

void *testFunction1(void *args)
{
	double *sum = (double *)malloc(sizeof(double));
	if(!sum) return NULL;
	*sum = cos(*(double *)args * PI / 180.0);
	free(args); // Allocated in producer.

	return (sum);
}

void *testFunction2(void *args)
{
	double *sum = (double *)malloc(sizeof(double));
	if(!sum) return NULL;
	*sum = tan(*(double *)args * PI / 180.0);
	free(args); // Allocated in producer.

	return (sum);
}

void *testFunction3(void *args)
{
	printf("Quarantine Days!\n");
	free(args); // Allocated in producer.

	return NULL;
}
