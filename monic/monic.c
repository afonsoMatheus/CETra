#include <stdio.h>
#include <stdlib.h>

#define CLU 2 // number of clusterings
#define MAX 4 // maximun sample size for vector alocation
#define FEA 3 // number of features 

float **weightAge(int* sizes);
float age(float last_w);

int *clusterIndex(int *y, int s, int clu, int *cont);

int *unique(int *y, int size, int *us);
void selectionSort(int arr[], int n);
void swap(int* xp, int* yp);

void main(int argc, char const *argv[]){

	//inputs of the two clusterings

	float X_1[][FEA] = {{20.0,30.0,15.0},{5.0,5.0,5.0},{40.0,50.0,40.0}};
	int y_1[] = {1,0,1};
	int size_1 = sizeof(y_1)/sizeof(*y_1);

	float X_2[][FEA] = {{20.0,30.0,15.0},{5.0,5.0,5.0},{40.0,50.0,40.0}, {60.0,60.0,60.0}};
	int y_2[] = {1,2,0,0};
	int size_2 = sizeof(y_2)/sizeof(*y_2);

	//getting clusters informations of each clustering

	int usize_1; 
	int *unique_c1 = unique(y_1, size_1, &usize_1);

	int usize_2;
	int *unique_c2 = unique(y_2, size_2, &usize_2);

	//assigning the weights of each sample for each clustering

	int sizes[CLU] = {size_1, size_2};
	float **age_w = weightAge(sizes);

	//getting the samples' indexes for each cluster in each clustering

	int *cont_1 = (int *) malloc(MAX * sizeof(int*));
	int **clu_index_1 = (int **) malloc(usize_1 * sizeof(int*));
	for (int i = 0; i < usize_1; ++i){
		clu_index_1[i] = clusterIndex(y_1, size_1, unique_c1[i], &cont_1[i]);
	}; 

	int *cont_2 = (int *) malloc(MAX * sizeof(int*));
	int **clu_index_2 = (int **) malloc(usize_2 * sizeof(int*));
	for (int i = 0; i < usize_2; ++i){
		clu_index_2[i] = clusterIndex(y_2, size_2, unique_c2[i], &cont_2[i]);
	};

};


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

int *clusterIndex(int *y, int s, int clu, int *cont){

	int *clu_index = malloc (sizeof (int) * 4);
	*cont = 0;

	int j = 0;
	for (int i = 0; i < s; ++i){
		if(y[i] == clu){
			clu_index[j] = i;
			j++;
			*cont = *cont + 1; 
		};
	};

	return clu_index;

};

int *unique(int *y, int size, int *us){

	int *clu_unique = (int*) malloc (sizeof (int) * 100);
	
	clu_unique[0] = y[0];

	int s = 1;
	int repeat;
	for (int i = 1; i < size; i++){

		repeat = 0;
		for (int j = 0; j < s; j++){

			if(y[i] == clu_unique[j]){
				repeat++;
			};
		};

		if (repeat < 1){
			clu_unique[s] = y[i];
			s++;
		};

	};

	clu_unique = (int*) realloc (clu_unique, sizeof (int) * s);
	*us = s;

	selectionSort(clu_unique, s);

	return clu_unique;

};
 
void selectionSort(int arr[], int n){

    int i, j, min_idx;
 
    for (i = 0; i < n - 1; i++) {
 
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
 
        swap(&arr[min_idx], &arr[i]);
    };
};

void swap(int* xp, int* yp){
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
};






