/* Author: Afonso Matheus                                                     */
/* Date: 2021                                                                 */
//----------------------------------------------------------------------------// 

#ifndef EXTRANS_H
#define EXTRANS_H

#include "General.h"

struct Transitions{

	int *deaths;
	int dea_size;

	struct Tuple *absors;
	int abs_size;

	struct Tuple *splits;
	int spl_size;

	struct SurvTuple *survs;
	int sur_size;

	struct Internal *inters;

	int *births; 
	int bir_size;
};

struct Internal{

	const char *intComp;
	const char *intSize;
	const char *intLocal;

};

//---------------------------------------------------------------------------

struct Transitions extTransitions(int ** clu_index_1, int ** clu_index_2, int *cont_1, int *cont_2, int *unique_c1, int *unique_c2, int usize_1, int usize_2, float *age_j, float **overlap_m, float **overlap_i);

int *getCandidates(struct Tuple *, int, int, int*);

#endif