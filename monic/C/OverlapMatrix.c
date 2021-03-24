/* Author: Afonso Matheus                                                      */
/* Date: 2021                                                                  */
//-----------------------------------------------------------------------------//                                                                          
//                                                                             //
// Script that contains the implementation of the overlap matrix builder       // 
//                                                                             //
//-----------------------------------------------------------------------------//

#include "OverlapMatrix.h"

/*
*
*	Func: 		
*		overlapMatrix(int **, int **, int *, int *, int, int, int*, int*, float *);
*	Args: 
*		Matrix of indexes for each cluster in clustering i, 
*		Matrix of indexes for each cluster in clustering j, 
*		Array of unique labels in clustering i, 
*		Array of unique labels in clustering j, 
*		Size of the unique labels array of clustering i, 
*		Size of the unique labels array of clustering j, 
*		Array with sizes of each cluster in clustering i, 
*		Array with sizes of each cluster in clustering j, 
*		Array with the aged weight of clustering j. 
*	Ret: 
*		None, detects the transitions of the clusters between the clustering i 
*		and the clustering j.
*
*/
float **overlapMatrix(int **clu_index_i, int **clu_index_j, int *unique_ci, int *unique_cj, int usize_i, int usize_j, int* cont_ci, int* cont_cj, float *age_j){

	float **overlaps = (float **) malloc(usize_i * sizeof(float*));
	for(int i = 0; i < usize_i; i++) overlaps[i] = (float *) malloc(usize_j * sizeof(float));	
	
	for (int i = 0; i < usize_i ; ++i){

		float sum_clusi = 0.0;

		for (int j = 0; j < usize_j; ++j){

			if (unique_cj[j] != -1){
				overlaps[i][j] = overlap(clu_index_i[i], cont_ci[i], clu_index_j[j], cont_cj[j], age_j);

			}else{

				overlaps[i][j] = 0.0;

			};
		};
	};
	
	return overlaps;
};

/*
*
*	Func: 		
*		overlap(int *, int, int *, int, float *);
*	Args: 
*		Array of indexes of a cluster from clustering i,
*		Size of the cluster from  clustering i,
*		Array of indexes of a cluster from clustering j,
*		Size of the cluster from  clustering j,
*		Array with the aged weight of clustering j. 
*	Ret: 
*		The overlap value between a cluster from clustering i and
*		a cluster from clustering j.
*
*/
float overlap(int *ci, int count_i, int *cj, int count_j, float *age_j){

	float inter_sum = 0.0;
	float clus_i = 0.0;

	for (int i = 0; i < count_i; ++i){
		clus_i = clus_i + age_j[ci[i]];

		for (int j = 0; j < count_j; ++j){
			if (ci[i] == cj[j]){
				inter_sum = inter_sum + age_j[ci[i]];
			};
		};	
	};

	return inter_sum/clus_i;

};