/* Author: Afonso Matheus   */
/* Date: 2021               */
//---------------------------------------------------------------------------

#ifndef UTIL_H
#define UTIL_H

#include <math.h>
#include "General.h"



//---------------------------------------------------------------------------

float mean(float **, int, int);

float stdev(float **, int, int);

int *unique(int *, int, int *);

void selectionSort(int *, int);

void swap(int*, int*);

int max(int *, int);

void concatenate(int *, int *, int *, int);

int *clusterIndex(int *, int, int, int *);

#endif