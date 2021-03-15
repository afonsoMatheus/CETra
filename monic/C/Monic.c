/* Author: Afonso Matheus   */
/* Date: 2021              */
//---------------------------------------------------------------------------

#include "Monic.h"

int CLU = 2; // number of clusterings
int MAX = 105; // maximun sample size for vector alocation
int FEA = 3; // number of features

void main(int argc, char const *argv[]){

	//inputs of the two clusterings

	struct Clustering *C_1 = readCsv("Clusterings/evo_cluster_0.csv");
	struct Clustering *C_2 = readCsv("Clusterings/evo_cluster_1.csv");

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

	struct Transitions TRANS = extTransitions(clu_index_1, clu_index_2, cont_1, cont_2, unique_c1, unique_c2, usize_1, usize_2,  age_w[1],overlap_m, overlap_i);

	//internal transitions

	intTransitions(&TRANS,  age_w, C_1, clu_index_1, cont_1, C_2, clu_index_2, cont_2);

	//showing transitions
	
	showTransitions(TRANS);

	free(clu_index_1);
	free(clu_index_2);
	free(cont_1);
	free(cont_2);
	
};

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

       row++;
    };

    C->s = row;

    fclose(fp);

    return C; 


};


void showTransitions(struct Transitions TRANS){
		
		printf("Deaths:\n");
		for (int i = 0; i < TRANS.dea_size; ++i){
			printf("C_%d -> DEATH \n", TRANS.deaths[i]);
		};
		printf("\n");

		printf("Survivals:\n");
		for (int i = 0; i < TRANS.sur_size; ++i){
			printf("C_%d -> C_%d\n", TRANS.survs[i].ci, TRANS.survs[i].cj);

			if(TRANS.inters[i].intSize != NULL){
				printf("	C_%d -> %s\n", TRANS.survs[i].ci, TRANS.inters[i].intSize);
			};
			if(TRANS.inters[i].intComp != NULL){
				printf("	C_%d -> %s\n", TRANS.survs[i].ci, TRANS.inters[i].intComp);
			};
			if(TRANS.inters[i].intLocal != NULL){
				printf("	C_%d -> %s\n", TRANS.survs[i].ci, TRANS.inters[i].intLocal);
			};
		
		};
		printf("\n");

		printf("Splits:\n");
		for (int i = 0; i < TRANS.spl_size; ++i){
			printf("C_%d -> C_%d\n", TRANS.splits[i].ci, TRANS.splits[i].cj);
		};
		printf("\n");

		printf("Unions:\n");
		for (int i = 0; i < TRANS.abs_size; ++i){
			printf("C_%d -> C_%d\n", TRANS.absors[i].ci, TRANS.absors[i].cj);
		};
		printf("\n");

		printf("Births:\n");
		for (int i = 0; i < TRANS.bir_size; ++i){
			printf("* -> C_%d\n", TRANS.births[i]);
		};
		printf("\n");
};






