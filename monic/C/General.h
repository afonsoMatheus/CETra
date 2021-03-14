/* Author: Afonso Matheus   */
/* Date: 2021              */
//---------------------------------------------------------------------------

#ifndef GENERAL_H
#define GENERAL_H

#include <stdio.h>
#include <stdlib.h>

//---------------------------------------------------------------------------

extern int CLU;
extern int MAX;
extern int FEA;

struct Clustering{
	float **X;
	int *y;
	int s;
};

struct Tuple{
	int ci;
	int cj;
};

struct SurvTuple{
	int ci;
	int cj;
	int i;
	int j;
};

#endif