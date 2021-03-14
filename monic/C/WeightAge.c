/* Author: Afonso Matheus   */
/* Date: 2021              */
//---------------------------------------------------------------------------

#include "WeightAge.h"

//---------------------------------------------------------------------------


float **weightAge(int* sizes){

	float **age_w = (float **) malloc(CLU * sizeof(float*));
	for(int i = 0; i < CLU; i++) age_w[i] = (float *)malloc(MAX * sizeof(float));

	for (int i = 0; i < sizes[0]; ++i){
		age_w[0][i] = 1.0;
	};

	for (int i = 1; i < CLU; ++i){
		for (int j = 0; j < sizes[i]; ++j){

			if(j < sizes[i-1]){
				age_w[i][j] = age(age_w[i-1][j]);
			}else{
				age_w[i][j] = 1.0;
			};

		};	
	};

	return age_w;
	
};

float age(float last_w){	
	return last_w*0.75;
};


