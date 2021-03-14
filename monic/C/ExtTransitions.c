/* Author: Afonso Matheus   */
/* Date: 2021              */
//---------------------------------------------------------------------------

#include "ExtTransitions.h"
#include "Util.h"
#include "OverlapMatrix.h"

//---------------------------------------------------------------------------

struct Transitions extTransitions(int ** clu_index_1, int ** clu_index_2, int *cont_1, int *cont_2, int *unique_c1, int *unique_c2, int usize_1, int usize_2, float *age_j, float **overlap_m, float **overlap_i) {

	struct Transitions trans;
	trans.deaths = (int *) malloc (usize_1 * sizeof(int));
	trans.splits = (struct Tuple *) malloc ((usize_1 + usize_2) * sizeof(struct Tuple));
	trans.spl_size = 0, trans.dea_size = 0;

	int *tracked = (int *) malloc (usize_2 * sizeof(int));
	struct Tuple *absors = (struct Tuple *) malloc ((usize_1 + usize_2) * sizeof(struct Tuple));

	int t = 0, a = 0;

	for (int i = 0; i < usize_1; ++i){

		int *split_cand = (int *) malloc (usize_2 * sizeof(int));
		split_cand[0] = -1;

		int *split_union = (int *) malloc (sizeof(int));

		int sc = 0, su = 0;

		int surv_cand = -1;

		for (int j = 0; j < usize_2; ++j){
			
			float mcell = overlap_m[i][j];

			if (mcell > 0.5){
				if (mcell > surv_cand){
					surv_cand = j;
				}else if(mcell == surv_cand){

					if (overlap_i[j][i] > overlap_i[surv_cand][i]){
						surv_cand = j;
					};

				};

			}else if(mcell > 0.25){

				split_cand[sc] = j;
				sc++;
				
				concatenate(split_union, &su, clu_index_2[j], cont_2[j]);

			};

		};

		if (surv_cand == -1 && split_cand[0] == -1){
			trans.deaths[trans.dea_size] = unique_c1[i];
			trans.dea_size++;
			
		}else if(split_cand[0] != -1){

			if (overlap(clu_index_1[i], cont_1[i], split_union, su, age_j) > 0.5){
				
				for (int j = 0; j < sc; ++j){

					trans.splits[trans.spl_size].ci = unique_c1[i];
					trans.splits[trans.spl_size].cj = unique_c2[split_cand[j]];
					trans.spl_size++;

					tracked[t] = unique_c2[split_cand[j]];
					t++;
				};


			}else{
				trans.deaths[trans.dea_size] = unique_c1[i];
				trans.dea_size++;
			};

		}else{

			absors[a].ci = unique_c1[i];
			absors[a].cj = unique_c2[surv_cand];

			a++;

		};
	
	};
	
	trans.absors = (struct Tuple *) malloc (a * sizeof(struct Tuple));
	trans.survs = (struct SurvTuple *) malloc ((usize_1) * sizeof(struct SurvTuple));
	trans.sur_size = 0, trans.abs_size = 0;

	for (int i = 0; i < usize_2; ++i){

		int al = 0;	
		int *absors_cand = getCandidates(absors, a, unique_c2[i], &al);	
		
		if (al > 1){

			for (int j = 0; j < al; ++j){

				trans.absors[j].ci = absors_cand[j];
				trans.absors[j].cj = unique_c2[i];
				trans.abs_size++;

				tracked[t] = unique_c2[i];
				t++;
				
			};
		 	
		}else if(absors_cand[0] == unique_c2[i]){
			trans.survs[trans.sur_size].ci = absors_cand[0];
			trans.survs[trans.sur_size].cj = unique_c2[i];
			

			for (int k = 0; k < usize_1; ++k){
				if (unique_c1[k] == absors_cand[0]){
					trans.survs[trans.sur_size].i = k;
					break;
				}
			}

			trans.survs[trans.sur_size].j = i;
			trans.sur_size++;

			tracked[t] = unique_c2[i];
			t++;

		}; 

	};

	trans.births = (int*) malloc(usize_2 * sizeof(int));

	trans.bir_size = 0;
	for (int i = 0; i < usize_2; ++i){
		for (int j = 0; j < t; ++j){
			if (tracked[j] == unique_c2[i]){
				break;
			}else if(j == t - 1 && unique_c2[i] != -1){
				trans.births[trans.bir_size] = unique_c2[i];
				trans.bir_size++;
			};
		};

	};
	
	return trans;
};

int *getCandidates(struct Tuple *absor_sur, int size, int j, int *s){

	int *absortion_cand = (int*) malloc(size * sizeof(int));

	int k = 0;
	for (int i = 0; i < size; ++i){
		if (absor_sur[i].cj == j){
			absortion_cand[k] = absor_sur[i].ci;
			k++;
		};
	};

	*s = k;

	return absortion_cand;

};