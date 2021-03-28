/* Author: Afonso Matheus                                                      */
/* Date: 2021                                                                  */
//-----------------------------------------------------------------------------//
//                                                                             //
// Script that contains the implementation of the external transitions         //
// detector                                                                    //
//                                                                             //
//-----------------------------------------------------------------------------//

#include "ExtTransitions.h"
#include "Util.h"
#include "OverlapMatrix.h"

/*
*
*	Func: 		
*		extTransitions(int **, int **, int * int *, int *, int *, int, int, float *, float **, float **);
*	Args: 
*		Matrix of indexes for each cluster in clustering i, 
*		Matrix of indexes for each cluster in clustering j,   
*		Array with sizes of each cluster in clustering i, 
*		Array with sizes of each cluster in clustering j,
*		Array of unique labels in clustering i, 
*		Array of unique labels in clustering j.
*		Size of the unique labels array of clustering i, 
*		Size of the unique labels array of clustering j, 
*		Array with the aged weight of clustering j,
*		Matrix with overlap values,
*		Matrix with inverted overlap values.
*	Ret: 
*		The external transictions between clustering i and clustering j. 
*
*/
struct Transitions extTransitions(int ** clu_index_i, int ** clu_index_j, int *cont_i, int *cont_j, int *unique_ci, int *unique_cj, int usize_i, int usize_j, float *age_j, float **overlap_m, float **overlap_i) {

	struct Transitions trans;
	trans.deaths = (int *) malloc (usize_i * sizeof(int));
	trans.splits = (struct Tuple *) malloc ((usize_i + usize_j) * sizeof(struct Tuple));
	trans.spl_size = 0, trans.dea_size = 0;

	int *tracked = (int *) malloc (usize_j * sizeof(int));
	struct Tuple *absors = (struct Tuple *) malloc ((usize_i + usize_j) * sizeof(struct Tuple));

	int t = 0, a = 0;

	for (int i = 0; i < usize_i; ++i){

		int *split_cand = (int *) malloc (usize_j * sizeof(int));
		split_cand[0] = -2;

		int *split_union = (int *) malloc (sizeof(int));

		int sc = 0, su = 0;

		int surv_cand = -2;

		for (int j = 0; j < usize_j; ++j){
			
			float mcell = overlap_m[i][j];

			if (mcell > 0.5){

				if(surv_cand == -2){
					surv_cand = j;

				}else if (mcell > overlap_m[i][surv_cand]){
					surv_cand = j;
				
				}else if(mcell == overlap_m[i][surv_cand]){

					if (overlap_i[j][i] > overlap_i[surv_cand][i]){
						surv_cand = j;
					};
				};

			}else if(mcell > 0.25){

				split_cand[sc] = j;
				sc++;
				
				concatenate(split_union, &su, clu_index_j[j], cont_j[j]);

			};

		};

		if (surv_cand == -2 && split_cand[0] == -2){
			trans.deaths[trans.dea_size] = unique_ci[i];
			trans.dea_size++;
			
		}else if(split_cand[0] != -2){

			if (overlap(clu_index_i[i], cont_i[i], split_union, su, age_j) > 0.5){
				
				for (int j = 0; j < sc; ++j){

					trans.splits[trans.spl_size].ci = unique_ci[i];
					trans.splits[trans.spl_size].cj = unique_cj[split_cand[j]];
					trans.spl_size++;

					/*
					if(unique_ci[i] != unique_cj[split_cand[j]]){
						trans.splits[trans.spl_size].ci = unique_ci[i];
						trans.splits[trans.spl_size].cj = unique_cj[split_cand[j]];
						trans.spl_size++;
					}else{
						trans.survs[trans.sur_size].ci = unique_ci[i];
						trans.survs[trans.sur_size].cj = unique_cj[split_cand[j]];

						trans.survs[trans.sur_size].j = split_cand[j];
						trans.sur_size++;

					};
					*/

					tracked[t] = unique_cj[split_cand[j]];
					t++;
				};


			}else{
				trans.deaths[trans.dea_size] = unique_ci[i];
				trans.dea_size++;
			};

		}else{

			absors[a].ci = unique_ci[i];
			absors[a].cj = unique_cj[surv_cand];
			a++;

		};
	
	};

	trans.absors = (struct Tuple *) malloc (a * sizeof(struct Tuple));
	trans.survs = (struct SurvTuple *) malloc ((usize_i) * sizeof(struct SurvTuple));
	trans.sur_size = 0, trans.abs_size = 0;

	for (int i = 0; i < usize_j; ++i){

		int al = 0;	
		int *absors_cand = getCandidates(absors, a, unique_cj[i], &al);	
		
		if (al > 1){

			for (int j = 0; j < al; ++j){

				trans.absors[trans.abs_size].ci = absors_cand[j];
				trans.absors[trans.abs_size].cj = unique_cj[i];
				trans.abs_size++;

				/*
				if(absors_cand[j] == unique_cj[i]){
					trans.survs[trans.sur_size].ci = absors_cand[j];
					trans.survs[trans.sur_size].cj = unique_cj[i];

					trans.survs[trans.sur_size].j = i;
					trans.sur_size++;

				}else{

					trans.absors[trans.abs_size].ci = absors_cand[j];
					trans.absors[trans.abs_size].cj = unique_cj[i];
					trans.abs_size++;

				};*/	

				tracked[t] = unique_cj[i];
				t++;
				
			};
		 	
		}else if(absors_cand[0] == unique_cj[i]){
			trans.survs[trans.sur_size].ci = absors_cand[0];
			trans.survs[trans.sur_size].cj = unique_cj[i];

			trans.survs[trans.sur_size].j = i;
			trans.sur_size++;

			tracked[t] = unique_cj[i];
			t++;

		}; 

	};

	for (int i = 0; i < trans.sur_size; ++i){
		for (int k = 0; k < usize_i; ++k){
			if (unique_ci[k] == trans.survs[i].ci){
				trans.survs[i].i = k;
				break;
			}
		}
	}

	trans.births = (int*) malloc(usize_j * sizeof(int));
	trans.bir_size = 0;
	
	for (int i = 0; i < usize_j; ++i){
		for (int j = 0; j < t; ++j){
			if (tracked[j] == unique_cj[i]){
				break;
			}else if(j == t - 1 && unique_cj[i] != -1){
				trans.births[trans.bir_size] = unique_cj[i];
				trans.bir_size++;
			};
		};

	};
	
	return trans;
};

/*
*
*	Func: 		
*		getCandidates(struct Tuple *, int, int, int *);
*	Args: 
*		Array of tuples with the clusters which suffered absortiona,
*		Size of absorved clusters,
*		Wanted cluster for absortion verification,
*		Current absortion size for cluster j.
*	Ret: 
*		The standart deviation of a n-dimensional sample set.
*
*/
int *getCandidates(struct Tuple *absor_sur, int size, int clu, int *s){

	int *absortion_cand = (int*) malloc(size * sizeof(int));

	int k = 0;
	for (int i = 0; i < size; ++i){
		if (absor_sur[i].cj == clu){
			absortion_cand[k] = absor_sur[i].ci;
			k++;
		};
	};

	*s = k;

	return absortion_cand;

};