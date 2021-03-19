/* Author: Afonso Matheus   */
/* Date: 2021              */
//---------------------------------------------------------------------------

#include "Util.h"
#include <math.h>

//---------------------------------------------------------------------------

float stdev(float **X, int row, int col){

	float m = mean(X, row, col);

	float sd = 0.0;
	for (int i = 0; i < row; ++i){
		for (int j = 0; j < col; ++j){
			sd += pow(X[i][j] - m, 2);
		};
	};


	return sqrt(sd/(row*col));

};

float mean(float **X, int row, int col){

	float sum = 0.0;
	for (int i = 0; i < row; ++i){
		for (int j = 0; j < col; ++j){
			sum += X[i][j];
		}
	}

	return sum/(row*col);

};

void concatenate(int *v1, int *s1, int *v2, int s2){

	if(*s1 == 0){

		v1 = (int*) realloc(v1, s2 * sizeof(int));

		for (int i = 0; i < s2; ++i){
			v1[i] = v2[i];
		};

		*s1 = s2;
		
	}else{

		v1 = (int*) realloc(v1, (*s1 + s2) * sizeof(int));

		int k = *s1;
		for (int i = 0; i < s2; ++i){
			v1[k] = v2[i];
			k++;
		};

		*s1 = k;

	};
};

int max(int *v, int count){

	int max_v = -1;
	for (int i = 0; i < count; ++i){
		if (v[i] > max_v){
			max_v = v[i];	
		};
	};

	return max_v;
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

int *clusterIndex(int *y, int s, int clu, int *cont){

	int *clu_index = malloc (sizeof (int) * MAX);
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