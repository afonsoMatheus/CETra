/* Author: Afonso Matheus   */
/* Date: 2021              */
//---------------------------------------------------------------------------

#include "OverlapMatrix.h"

//---------------------------------------------------------------------------


float **overlapMatrix(int **clu_index_1, int **clu_index_2, int *unique_c1, int *unique_c2, int usize_i, int usize_j, int* cont_ci, int* cont_cj, float *age_j){

	float **overlaps = (float **) malloc(usize_i * sizeof(float*));
	for(int i = 0; i < usize_i; i++) overlaps[i] = (float *) malloc(usize_j * sizeof(float));	
	
	for (int i = 0; i < usize_i ; ++i){

		float sum_clusi = 0.0;

		for (int j = 0; j < usize_j; ++j){

			if (unique_c2[j] != -1){
				overlaps[i][j] = overlap(clu_index_1[i], cont_ci[i], clu_index_2[j], cont_cj[j], age_j);
			}else{

				overlaps[i][j] = 0.0;

			};
		};
	};
	
	return overlaps;
};

float overlap(int *c1, int count_i, int *c2, int count_j, float *age_j){

	float inter_sum = 0.0;
	float clus_i = 0.0;

	for (int i = 0; i < count_i; ++i){
		clus_i = clus_i + age_j[c1[i]];

		for (int j = 0; j < count_j; ++j){
			if (c1[i] == c2[j]){
				inter_sum = inter_sum + age_j[c1[i]];
			};
		};	
	};

	return inter_sum/clus_i;

};