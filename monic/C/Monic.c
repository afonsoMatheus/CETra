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

	TRANS.inters = (struct Internal*) malloc(TRANS.sur_size *sizeof(struct Internal));

	for (int i = 0; i < TRANS.sur_size; ++i){

		TRANS.inters[i].intSize = sizeTrans(clu_index_1[TRANS.survs[i].i], cont_1[TRANS.survs[i].i], clu_index_2[TRANS.survs[i].j], cont_2[TRANS.survs[i].j], age_w[0], age_w[1]);

		TRANS.inters[i].intComp = compTrans(clu_index_1[TRANS.survs[i].i], cont_1[TRANS.survs[i].i], clu_index_2[TRANS.survs[i].j], cont_2[TRANS.survs[i].j], C_1->X, C_2->X);

		TRANS.inters[i].intLocal = locTrans(clu_index_1[TRANS.survs[i].i], cont_1[TRANS.survs[i].i], clu_index_2[TRANS.survs[i].j], cont_2[TRANS.survs[i].j], C_1->X, C_2->X);

	};

	printf("%s\n", TRANS.inters[0].intSize);
	printf("%s\n", TRANS.inters[0].intComp);
	printf("%s\n", TRANS.inters[0].intLocal);

	//showing transictions
	
	showTransictions(TRANS.survs, TRANS.sur_size, TRANS.splits, TRANS.spl_size, TRANS.births, TRANS.bir_size, TRANS.absors, TRANS.abs_size, TRANS.deaths, TRANS.dea_size);

	free(clu_index_1);
	free(clu_index_2);
	free(cont_1);
	free(cont_2);
	
};


void showTransictions(struct SurvTuple *survs, int sur, struct Tuple *splits, int spl, int *births, int bir,  struct Tuple *absors, int abs, int *deaths, int dea){
		
		printf("Deaths:\n");
		for (int i = 0; i < dea; ++i){
			printf("C_%d -> DEATH \n", deaths[i]);
		};
		printf("\n");

		printf("Survivals:\n");
		for (int i = 0; i < sur; ++i){
			printf("C_%d -> C_%d\n", survs[i].ci, survs[i].cj);
		};
		printf("\n");

		printf("Splits:\n");
		for (int i = 0; i < spl; ++i){
			printf("C_%d -> C_%d\n", splits[i].ci, splits[i].cj);
		};
		printf("\n");

		printf("Unions:\n");
		for (int i = 0; i < abs; ++i){
			printf("C_%d -> C_%d\n", absors[i].ci, absors[i].cj);
		};
		printf("\n");

		printf("Births:\n");
		for (int i = 0; i < bir; ++i){
			printf("* -> C_%d\n", births[i]);
		};
		printf("\n");
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




