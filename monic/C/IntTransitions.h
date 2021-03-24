/* Author: Afonso Matheus                                                     */
/* Date: 2021                                                                 */
//----------------------------------------------------------------------------// 

#ifndef INTRANS_H
#define INTRANS_H

#include "ExtTransitions.h"

//---------------------------------------------------------------------------

void intTransitions(struct Transitions *, float *, float *, struct Clustering, int **, int *, struct Clustering, int **, int *);

const char *sizeTrans(int *, int, int *, int, float*, float *);

const char *compTrans(int *, int, int *, int, float **, float **);

const char *locTrans(int *, int, int *, int, float **, float **);

#endif