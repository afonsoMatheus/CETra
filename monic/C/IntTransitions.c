/* Author: Afonso Matheus   */
/* Date: 2021              */
//---------------------------------------------------------------------------

#include "IntTransitions.h"
#include "Util.h"

//---------------------------------------------------------------------------

void intTransitions(struct Transitions *TRANS, float *age_i, float *age_j, struct Clustering C_1, int **clu_index_1, int *cont_1, struct Clustering C_2, int **clu_index_2, int *cont_2){

	TRANS->inters = (struct Internal*) malloc(TRANS->sur_size *sizeof(struct Internal));

	for (int i = 0; i < TRANS->sur_size; ++i){

		TRANS->inters[i].intSize = sizeTrans(clu_index_1[TRANS->survs[i].i], cont_1[TRANS->survs[i].i], clu_index_2[TRANS->survs[i].j], cont_2[TRANS->survs[i].j], age_i, age_j);

		TRANS->inters[i].intComp = compTrans(clu_index_1[TRANS->survs[i].i], cont_1[TRANS->survs[i].i], clu_index_2[TRANS->survs[i].j], cont_2[TRANS->survs[i].j], C_1.X, C_2.X);

		TRANS->inters[i].intLocal = locTrans(clu_index_1[TRANS->survs[i].i], cont_1[TRANS->survs[i].i], clu_index_2[TRANS->survs[i].j], cont_2[TRANS->survs[i].j], C_1.X, C_2.X);

	};


}; 

const char *locTrans(int *clu_ti, int size_ti, int *clu_tj, int size_tj, float **X_ti, float **X_tj){
	
	float **Xi = (float **) malloc (size_ti * sizeof(float*));
	for(int i = 0; i < size_ti; i++) Xi[i] = (float *)malloc(FEA * sizeof(float));
	
	for (int i = 0; i < size_ti; ++i){
		for (int j = 0; j < FEA; ++j){
			Xi[i][j] = X_ti[clu_ti[i]][j];
		};
	};

	float **Xj = (float **) malloc (size_tj * sizeof(float*));
	for(int i = 0; i < size_tj; i++) Xj[i] = (float *)malloc(FEA * sizeof(float));
	
	for (int i = 0; i < size_tj; ++i){
		for (int j = 0; j < FEA; ++j){
			Xj[i][j] = X_tj[clu_tj[i]][j];
		};
	};

	float mean_i = mean(Xi, size_ti, FEA);
	float mean_j = mean(Xj, size_tj, FEA);

	if(mean_i - mean_j > 3){
		return "Localization";
	}else{
		return NULL;
	};

};

const char *compTrans(int *clu_ti, int size_ti, int *clu_tj, int size_tj, float **X_ti, float **X_tj){

	float **Xi = (float **) malloc (size_ti * sizeof(float*));
	for(int i = 0; i < size_ti; i++) Xi[i] = (float *)malloc(FEA * sizeof(float));
	
	for (int i = 0; i < size_ti; ++i){
		for (int j = 0; j < FEA; ++j){
			Xi[i][j] = X_ti[clu_ti[i]][j];
		};
	};


	float **Xj = (float **) malloc (size_tj * sizeof(float*));
	for(int i = 0; i < size_tj; i++) Xj[i] = (float *)malloc(FEA * sizeof(float));
	
	for (int i = 0; i < size_tj; ++i){
		for (int j = 0; j < FEA; ++j){
			Xj[i][j] = X_tj[clu_tj[i]][j];

		};
	};

	float std_i = stdev(Xi, size_ti, FEA);
	float std_j = stdev(Xj, size_tj, FEA);

	if (std_j < std_i - 0.05){
		return "Compaction";
	}else if(std_j > std_i + 0.05){
		return "Difusion";
	}else{
		return NULL;
	};

};

const char *sizeTrans(int *clu_ti, int size_ti, int *clu_tj, int size_tj, float *age_ti, float *age_tj){

	float sum_i = 0;
	for (int i = 0; i < size_ti; ++i){
		sum_i = sum_i + age_ti[clu_ti[i]];
	};

	float sum_j = 0;
	for (int j = 0; j < size_tj; ++j){
		sum_j = sum_j + age_tj[clu_tj[j]];
	};

	if (sum_i * 0.75 > sum_j + 10){
		return "Shrinked";		
	}else if(sum_j > (sum_i * 0.75) + 10){
		return "Expanded";
	}else{
		return NULL;
	};

};