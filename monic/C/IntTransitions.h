/* Author: Afonso Matheus   */
/* Date: 2021              */
//---------------------------------------------------------------------------

#ifndef INTRANS_H
#define INTRANS_H

#include "General.h"

struct Internal{

	const char *intComp;
	const char *intSize;
	const char *intLocal;

};

//---------------------------------------------------------------------------

const char *sizeTrans(int *, int, int *, int, float*, float *);

const char *compTrans(int *, int, int *, int, float **, float **);

const char *locTrans(int *, int, int *, int, float **, float **);

#endif