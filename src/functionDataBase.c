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
#include "/home/vamoirid/Desktop/git/Real-Time-Embedded-Systems/inc/functionDataBase.h"

void *testFunction0(void *args)
{
	double *sum = (double *)malloc(sizeof(double));
	if(!sum) return NULL;
	*sum = sin(*(double *)args * PI / 180.0);
	//printf("sin(%.3f) = %.3f\n", *(double *)args, *sum);
	free(args);

	return (sum);
}

void *testFunction1(void *args)
{
	double *sum = (double *)malloc(sizeof(double));
	if(!sum) return NULL;
	*sum = cos(*(double *)args * PI / 180.0);
	//printf("cos(%.3f) = %.3f\n", *(double *)args, *sum);
	free(args);

	return (sum);
}

void *testFunction2(void *args)
{
	double *sum = (double *)malloc(sizeof(double));
	if(!sum) return NULL;
	*sum = tan(*(double *)args * PI / 180.0);
	//printf("tan(%.3f) = %.3f\n", *(double *)args, *sum);
	free(args);

	return (sum);
}

void *testFunction3(void *args)
{
	//fprintf(stdout,"Quarantine Days!\n");

	return args;
}