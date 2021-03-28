/* Author: Afonso Matheus                                                     */
/* Date: 2021                                                                 */
//----------------------------------------------------------------------------// 

#ifndef MONIC_H
#define MONIC_H

#include <string.h>
#include <time.h>
#include "General.h"
#include "Util.h"
#include "OverlapMatrix.h"
#include "WeightAge.h"
#include "IntTransitions.h"
#include "ExtTransitions.h"

//---------------------------------------------------------------------------

void monic(struct Clustering C_1, struct Clustering C_2, float *age_i, float *age_j);

float **retrieveWAge(struct Clustering *C);

struct Clustering *retrieveClusterings();

void showTransitions(struct Transitions); 

struct Clustering readCsv(char*);

#endif