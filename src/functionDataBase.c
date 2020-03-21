/*
 *******************************************************************************
 * Author: Vasileios Amoiridis                                                 *
 * Filename: functionDataBase.c                                                *
 * Date: Mar 20 18:15                                                          *
 *******************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "/home/vamoirid/Desktop/git/Real-Time-Embedded-Systems/inc/functionDataBase.h"

void* testFunction0(void *args)
{
	double i, tot_sum = 0, *sum;

	sum = (double *)malloc(sizeof(double));
	if(!sum) return NULL;

	*sum = sin(*(double *)args * PI / 180);
	////for(i = 0; i < 360; i++) tot_sum += sin(i);
	//sum = &tot_sum; LATHOS!!!!!!!!!!!!
	printf("0. sin(%.0f) = %.3f\n",*(double *)args, *(double *)sum);

	return (sum);
}

void* testFunction1(void *args)
{
	double i, tot_sum = 0, *sum;

	sum = (double *)malloc(sizeof(double));
	if(!sum) return NULL;

	*sum = cos(*(double *)args * PI / 180);
	////for(i = 0; i < 360; i++) tot_sum += cos(i);
	//sum = &tot_sum;
	printf("1. cos(%.0f) = %.3f\n",*(double *)args, *(double *)sum);

	return (sum);
}

void* testFunction2(void *args)
{
	double i, tot_sum = 0, *sum;

	sum = (double *)malloc(sizeof(double));
	if(!sum) return NULL;

	*sum = tan(*(double *)args * PI / 180);
	////for(i = 0; i < 360; i++) tot_sum += tan(i);
	//sum = &tot_sum;
	printf("2. tan(%.0f) = %.3f\n",*(double *)args, *(double *)sum);

	return (sum);
}

void* testFunction3(void *args)
{
	double i, *sum, tot_sum = 0;
	//for(i = 0; i < 360; i++) tot_sum += sin(i)*cos(i);
	sum = &tot_sum;
	printf("3. sin()*cos() = %.3f\n",*(double *)sum);

	return (sum);
}

void* testFunction4(void *args)
{
	double i, *sum, tot_sum = 0;
	//for(i = 0; i < 360; i++) tot_sum += sin(i)*tan(i);
	sum = &tot_sum;
	printf("4. sin()*tan() = %.3f\n",*(double *)sum);

	return (sum);
}

void* testFunction5(void *args)
{
	double i, *sum, tot_sum = 0;	
	//for(i = 0; i < 360; i++) tot_sum += cos(i)*tan(i);
	sum = &tot_sum;
	printf("5. cos()*tan() = %.3f\n",*(double *)sum);

	return (sum);
}

void* testFunction6(void *args)
{
	double i, *sum, tot_sum = 0;
	//for(i = 0; i < 360; i++) tot_sum += sin(i)*sin(i);
	sum = &tot_sum;
	printf("6. sin()*sin() = %.3f\n",*(double *)sum);

	return (sum);
}

void* testFunction7(void *args)
{
	double i, *sum, tot_sum = 0;
	//for(i = 0; i < 360; i++) tot_sum += cos(i)*cos(i);
	sum = &tot_sum;
	printf("7. cos()*cos() = %.3f\n",*(double *)sum);

	return (sum);
}

void* testFunction8(void *args)
{
	double i, *sum, tot_sum = 0;	
	//for(i = 0; i < 360; i++) tot_sum += tan(i)*tan(i);
	sum = &tot_sum;
	printf("8. tan()*tan() = %.3f\n",*(double *)sum);

	return (sum);
}

void* testFunction9(void *args)
{
	double i, *sum, tot_sum = 0;
	//for(i = 0; i < 360; i++) tot_sum += sin(i)*cos(i)*tan(i);
	sum = &tot_sum;
	printf("9. sin()*cos()*tan() = %.3f\n",*(double *)sum);

	return (sum);
}