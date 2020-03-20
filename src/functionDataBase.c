/*
 *******************************************************************************
 * Author: Vasileios Amoiridis                                                 *
 * Filename: prod-cons.c                                                       *
 * Date: Mar 20 18:15                                                          *
 *******************************************************************************
 */
#include <stdio.h>
#include <math.h>
#include "/home/vamoirid/Desktop/git/Real-Time-Embedded-Systems/inc/functionDataBase.h"

void* testFunction0(void *sum)
{
	double i, tot_sum = 0;	
	for(i = 0; i < 360; i++) tot_sum += sin(i);
	sum = &tot_sum;
	printf("testFunction0\n");

	return (sum);
}

void* testFunction1(void *sum)
{
	double i, tot_sum = 0;
	for(i = 0; i < 360; i++) tot_sum += cos(i);
	sum = &tot_sum;
	printf("testFunction1\n");

	return (sum);
}

void* testFunction2(void *sum)
{
	double i, tot_sum = 0;	
	for(i = 0; i < 360; i++) tot_sum += tan(i);
	sum = &tot_sum;
	printf("testFunction2\n");

	return (sum);
}

void* testFunction3(void *sum)
{
	double i, tot_sum = 0;
	for(i = 0; i < 360; i++) tot_sum += sin(i)*cos(i);
	sum = &tot_sum;
	printf("testFunction3\n");

	return (sum);
}

void* testFunction4(void *sum)
{
	double i, tot_sum = 0;
	for(i = 0; i < 360; i++) tot_sum += sin(i)*tan(i);
	sum = &tot_sum;
	printf("testFunction4\n");

	return (sum);
}

void* testFunction5(void *sum)
{
	double i, tot_sum = 0;	
	for(i = 0; i < 360; i++) tot_sum += cos(i)*tan(i);
	sum = &tot_sum;
	printf("testFunction5\n");

	return (sum);
}

void* testFunction6(void *sum)
{
	double i, tot_sum = 0;
	for(i = 0; i < 360; i++) tot_sum += sin(i)*sin(i);
	sum = &tot_sum;
	printf("testFunction6\n");


	return (sum);
}

void* testFunction7(void *sum)
{
	double i, tot_sum = 0;
	for(i = 0; i < 360; i++) tot_sum += cos(i)*cos(i);
	sum = &tot_sum;
	printf("testFunction7\n");


	return (sum);
}

void* testFunction8(void *sum)
{
	double i, tot_sum = 0;	
	for(i = 0; i < 360; i++) tot_sum += tan(i)*tan(i);
	sum = &tot_sum;
	printf("testFunction8\n");

	return (sum);
}

void* testFunction9(void *sum)
{
	double i, tot_sum = 0;
	for(i = 0; i < 360; i++) tot_sum += sin(i)*cos(i)*tan(i);
	sum = &tot_sum;
	printf("testFunction9\n");

	return (sum);
}