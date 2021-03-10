#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define CLU 2 // number of clusterings
#define MAX 105 // maximun sample size for vector alocation
#define FEA 3 // number of features

struct Clustering{
	float **X;
	int *y;
	int s;
};

struct Tuple{
	char ci;
	char cj;
};

float **weightAge(int* sizes);
float age(float last_w);

int *clusterIndex(int *y, int s, int clu, int *cont);

int *unique(int *y, int size, int *us);
void selectionSort(int arr[], int n);
void swap(int* xp, int* yp);

int max(int *v, int count);
float **overlapMatrix(int **clu_index_1, int **clu_index_2, int *unique_c1, int *unique_c2, int usize_i, int usize_j, int* cont_ci, int* cont_cj, float *age_j);
float overlap(int *c1, int count_i, int *c2, int count_j, float *age_j);

void concatenate(int *v1, int *s1, int *v2, int s2);
char *getCandidates(struct Tuple *absor_sur, int size, char j, int *s);


struct Clustering *readCsv(char* path){


	struct Clustering *C = (struct Clustering *) malloc(sizeof(struct Clustering));
	C->X = (float **) malloc(MAX * sizeof(float*));
	for(int i = 0; i < MAX; i++) C->X[i] = (float *)malloc(FEA * sizeof(float));
	
	C->y = (int *) malloc(MAX * sizeof(int));

	FILE* fp = fopen(path, "r");
	char buffer[1024];
	int row = 0;
	int col = 0;
	int c = 0;

	while(fgets(buffer, 1024, fp)){

		col = 0;
		c = 0;
		
		
		char *value = strtok(buffer, ",");

		while (value) {

			if (col == 0){
				C->y[row] = atoi(value);

			}else{

				C->X[row][c] = atof(value);
				c++;
			};

            value = strtok(NULL, ", "); 
            col++; 
       };

       //printf("%d\n", C->y[row]);

       //printf("%f %f %f\n", C->X[row][0], C->X[row][1], C->X[row][2]); 

       row++;
    };

    C->s = row;

    //printf("%d\n", row);

    fclose(fp);

    return C; 


};


void main(int argc, char const *argv[]){

	//inputs of the two clusterings

	struct Clustering *C_1 = readCsv("evo_cluster_0.csv");
	struct Clustering *C_2 = readCsv("evo_cluster_1.csv");

	//getting clusters informations of each clustering

	int size_1 = C_1->s;
	int size_2 = C_2->s;

	int usize_1; 
	int *unique_c1 = unique(C_1->y, size_1, &usize_1);

	int usize_2;
	int *unique_c2 = unique(C_2->y, size_2, &usize_2);

	//assigning the weights of each sample for each clustering

	int sizes[] = {size_1, size_2};
	float **age_w = weightAge(sizes);

	//getting the samples' indexes for each cluster in each clustering

	int *cont_1 = (int *) malloc(MAX * sizeof(int*));
	int **clu_index_1 = (int **) malloc(usize_1 * sizeof(int*));
	for (int i = 0; i < usize_1; ++i){
		clu_index_1[i] = clusterIndex(C_1->y, size_1, unique_c1[i], &cont_1[i]);
	}; 

	int *cont_2 = (int *) malloc(MAX * sizeof(int*));
	int **clu_index_2 = (int **) malloc(usize_2 * sizeof(int*));
	for (int i = 0; i < usize_2; ++i){
		clu_index_2[i] = clusterIndex(C_2->y, size_2, unique_c2[i], &cont_2[i]);
	};

	//building overlapping matrix

	float **overlap_m = overlapMatrix(clu_index_1, clu_index_2, unique_c1, unique_c2, usize_1, usize_2, cont_1 , cont_2, age_w[1]);
	float **overlap_i = overlapMatrix(clu_index_2, clu_index_1, unique_c2, unique_c1, usize_2, usize_1, cont_2 , cont_1, age_w[1]);

	//external transitions

	char *deaths, *tracked = (char *) malloc ((usize_1 + usize_2) * sizeof(char));
	struct Tuple *splits, *absors = (struct Tuple *) malloc ((usize_1 + usize_2) * sizeof(struct Tuple));

	int d, s, a, t = 0;



	for (int i = 0; i < usize_1; ++i){

		int *split_cand;
		split_cand[0] = -1;

		int *split_union = (int *) malloc (sizeof(int));

		int sc, su = 0;
		int surv_cand = -1;

		for (int j = 0; j < usize_2; ++j){
			
			float mcell = overlap_m[i][j];

			if (mcell > 0.5){
				if (mcell > surv_cand){
					surv_cand = j;
				}else if(mcell == surv_cand){

					if (overlap_i[j][i] > overlap_i[surv_cand][i]){
						surv_cand = unique_c2[j];
					};

				};

			}else if(mcell > 0.25){

				split_cand[sc] = j;
				sc++;
				
				concatenate(split_union, &su, clu_index_2[j], cont_2[j]);

				printf("aqui %d\n", su);

				for (int k = 0; k < su; ++k){
					printf("%d\n", split_union[k]);
				}

			};

		};

		/*

		if (surv_cand == -1 && split_cand[0] == -1){
			deaths[d] = (char) unique_c1[i];
			d++;
			
		}else if(split_cand[0] != -1){

			if (overlap(clu_index_1[i], cont_1[i], split_union, su, age_w[1]) > 0.5){
				
				for (int j = 0; j < sc; ++j){
					splits[s].ci = (char) unique_c1[i];
					splits[s].cj = (char) split_cand[j];
					s++;

					tracked[t] = (char) split_cand[j];
					t++;
				};


			}else{
				deaths[d] = (char) unique_c1[i];
				d++;
			};

		}else{

			absors[a].ci = (char) unique_c1[i];
			absors[a].cj = (char) surv_cand;
			a++;

		};

		*/
		
	};

	/*

	struct Tuple *absors_list = (struct Tuple *) malloc ((usize_1 + usize_2) * sizeof(struct Tuple));
	char *survs = (char *) malloc ((usize_1 + usize_2) * sizeof(char));
	int sur = 0;

	for (int i = 0; i < usize_2; ++i){

		int al;
		char *absors_cand = getCandidates(absors, a, unique_c2[i], &al);

		if (al > 1){

			for (int j = 0; j < al; ++i){

				absors_list[j].ci = absors_cand[j];
				absors_list[j].cj = unique_c2[i];

				tracked[t] = (char) unique_c2[i];
				t++;
				
			};
		 	
		}else if (absors_cand[0] == unique_c2[i]){
			survs[sur] = unique_c2[i];
			sur++;
			tracked[t] = (char) unique_c2[i];
			t++;

		}; 

	};

	printf("%c\n", survs[0]);

	*/

};

char *getCandidates(struct Tuple *absor_sur, int size, char j, int *s){

	char *absortion_cand = (char*) malloc(size * sizeof(char));

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

void concatenate(int *v1, int *s1, int *v2, int s2){

	if(*s1 == 0){

		v1 = (int*) realloc(v1, MAX * sizeof(int));

		//v1 = (int*) realloc(v1, s2 * sizeof(int));

		for (int i = 0; i < s2; ++i){
			v1[i] = v2[i];
		};

		*s1 = s2;
		
	}else{

		//v1 = (int*) realloc(v1, (*s1 + s2) * sizeof(int));

		int k = *s1;
		for (int i = 0; i < s2; ++i){
			v1[k] = v2[i];
			k++;
		};

		*s1 = k;

	};

	

};


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


int max(int *v, int count){

	int max_v = -1;
	for (int i = 0; i < count; ++i){
		if (v[i] > max_v){
			max_v = v[i];	
		};
	};

	return max_v;
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






