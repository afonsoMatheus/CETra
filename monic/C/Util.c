/* Author: Afonso Matheus                                                      */
/* Date: 2021                                                                  */
//-----------------------------------------------------------------------------//
//                                                                             //
// Script that contains the implementation of utility functions used between   //
// the main files                                                              //
//                                                                             //
//-----------------------------------------------------------------------------//

#include "Util.h"
#include <math.h>

/*
*
*	Func: 		
*		stdev(float **, int, int);
*	Args: 
*		Matrix of n-dimensional samples,
*		Size of rows,
*		Size of columns.
*	Ret: 
*		The standart deviation of a n-dimensional sample set.
*
*/
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

/*
*
*	Func: 		
*		mean(float **, int, int);
*	Args: 
*		Matrix of n-dimensional samples,
*		Size of rows,
*		Size of columns.
*	Ret: 
*		The mean of a n-dimensional sample set.
*
*/
float mean(float **X, int row, int col){

	float sum = 0.0;
	for (int i = 0; i < row; ++i){
		for (int j = 0; j < col; ++j){
			sum += X[i][j];
		}
	}

	return sum/(row*col);

};

/*
*
*	Func: 		
*		concatenate(int *, int *, int *, int);
*	Args: 
*		First array of values,
*		Size of the first array,
*		Second array of values,
*		Size of the second array.
*	Ret: 
*		None, updates the first array making an union with the
*		elements of the second array.
*
*/
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

/*
*
*	Func: 		
*		max(int *, int);
*	Args: 
*		Array of values,
*		Size of array.
*	Ret: 
*		The max value of an array.
*
*/
int max(int *v, int count){

	int max_v = -1;
	for (int i = 0; i < count; ++i){
		if (v[i] > max_v){
			max_v = v[i];	
		};
	};

	return max_v;
};

/*
*
*	Func: 		
*		unique(int *, int, int *);
*	Args: 
*		Array with the label of each sample,
*		Size of the label array,
*		Current size of the unique labels array.
*	Ret: 
*		An array with the unique values of labels for a clustering.
*
*/
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

/*
*
*	Func: 		
*		selectionSort(int *, int);
*	Args: 
*		Array of values,
*		Size of array.
*	Ret: 
*		None, returns the sorted array.
*
*/ 
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

/*
*
*	Func: 		
*		swap(int* , int*);
*	Args: 
*		Address of index 1,
*		Address of index 2.
*	Ret: 
*		None, swaps indexes addresses.
*
*/ 
void swap(int* xp, int* yp){
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
};

/*
*
*	Func: 		
*		clusterIndex(int *, int, int, int *);
*	Args: 
*		Array with unique labels,
*		Size of the array with unique labels,
*		Wanted cluster label,
*		Current size of the wanted cluster. 
*	Ret: 
*		An array with the wanted clusters indexes.
*
*/ 
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