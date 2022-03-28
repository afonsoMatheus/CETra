/* Author: Afonso Matheus                                                      */
/* Date: 2021                                                                  */
//-----------------------------------------------------------------------------//                                                                          
//                                                                             //
// Script that contains the implementation of the clustering weight assigner   // 
//                                                                             //
//-----------------------------------------------------------------------------//

#include "WeightAge.h"

/*
*
*	Func: 		
*		weightAge(struct Clustering *);
*	Args: 
*		Array of clusterings. 
*	Ret: 
*		Matrix of float, the assigned values for each sample in each clustering.
*
*/
float **weightAge(struct Clustering *C){

	float **age_w = (float **) malloc(CLU * sizeof(float*));
	for(int i = 0; i < CLU; i++) age_w[i] = (float *)malloc(C[i].s * sizeof(float));

	for (int i = 0; i < C[0].s; ++i){
		age_w[0][i] = 1.0;
	};

	for (int i = 1; i < CLU; ++i){
		for (int j = 0; j < C[i].s; ++j){
			if(j < C[i-1].s){
				if(C[i-1].y[j] == -1){
					age_w[i][j] = 0;
				}else{
					age_w[i][j] = age(age_w[i-1][j]);
				}
			}else{
				age_w[i][j] = 1.0;
			};
		};

	};

	return age_w;
	
};

/*
*
*	Func: 		
*		age(float);
*	Args: 
*		A float value.
*	Ret: 
*		The aged (devaluation) float value. 
*
*/
float age(float last_w){	
	return last_w*0.75;
};


