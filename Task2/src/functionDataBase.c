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

void *timerFcn0(void *args)
{
	double *sum = (double *)malloc(sizeof(double));
	if(!sum) return NULL;
	*sum = sin(*(double *)args * PI / 180.0);

	return (sum);
}

void *timerFcn1(void *args)
{
	double *sum = (double *)malloc(sizeof(double));
	if(!sum) return NULL;
	*sum = cos(*(double *)args * PI / 180.0);

	return (sum);
}

void *timerFcn2(void *args)
{
	double *sum = (double *)malloc(sizeof(double));
	if(!sum) return NULL;
	*sum = tan(*(double *)args * PI / 180.0);

	return (sum);
}

void *timerFcn3(void *args)
{
	printf("Quarantine Days!\n");

	return NULL;
}
