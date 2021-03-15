/* Author: Afonso Matheus   */
/* Date: 2021              */
//---------------------------------------------------------------------------

#ifndef INTRANS_H
#define INTRANS_H

#include "ExtTransitions.h"

//---------------------------------------------------------------------------

void intTransitions(struct Transitions *TRANS, float **age_w, struct Clustering *C_1, int **clu_index_1, int *cont_1, struct Clustering *C_2, int **clu_index_2, int *cont_2);

const char *sizeTrans(int *, int, int *, int, float*, float *);

const char *compTrans(int *, int, int *, int, float **, float **);

const char *locTrans(int *, int, int *, int, float **, float **);

#endif