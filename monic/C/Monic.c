/* Author: Afonso Matheus                                                     */
/* Date: 2021                                                                 */
//----------------------------------------------------------------------------//                                                                          
//                                                                            //
// Script that contains the implementation of cluster's transiction detection // 
// between two n-dimensional clusterings (MONIC)                              //
//                                                                            //
//----------------------------------------------------------------------------//

#include "Monic.h"

/*Variables used between all modules*/

int CLU = 3; // number of clusterings
int MAX = 105; // maximun sample size for vector alocation
int FEA = 3; // number of features

/*
*
*	Func: 		
*		monic(struct Clustering, struct Clustering, float *, float *);
*	Args: 
*		Clustering in time i, 
*		Clustering in time j, 
*		The weights assigned to samples in time i, 
*		The weights assigned to samples in time j. (j > i)
*	Ret: 
*		None, detects the transitions of the clusters between the clustering i 
*		and the clustering j.
*
*/
void monic(struct Clustering C_i, struct Clustering C_j, float *age_i, float *age_j){

	int usize_i; 
	int *unique_ci = unique(C_i.y, C_i.s, &usize_i);

	int usize_j;
	int *unique_cj = unique(C_j.y, C_j.s, &usize_j);

	//getting the samples' indexes for each cluster in each clustering

	int *cont_i = (int *) malloc(MAX * sizeof(int));
	int **clu_index_i = (int **) malloc(usize_i * sizeof(int*));
	for (int i = 0; i < usize_i; ++i){
		clu_index_i[i] = clusterIndex(C_i.y, C_i.s, unique_ci[i], &cont_i[i]);
	}; 

	int *cont_j = (int *) malloc(MAX  * sizeof(int));
	int **clu_index_j = (int **) malloc(usize_j * sizeof(int*));
	for (int i = 0; i < usize_j; ++i){
		clu_index_j[i] = clusterIndex(C_j.y, C_j.s, unique_cj[i], &cont_j[i]);
	};

	//building overlapping matrix

	float **overlap_m = overlapMatrix(clu_index_i, clu_index_j, unique_ci, unique_cj, usize_i, usize_j, cont_i , cont_j, age_j);
	float **overlap_i = overlapMatrix(clu_index_j, clu_index_i, unique_cj, unique_ci, usize_j, usize_i, cont_j , cont_i, age_j);

	//external transitions

	struct Transitions TRANS = extTransitions(clu_index_i, clu_index_j, cont_i, cont_j, unique_ci, unique_cj, usize_i, usize_j, age_j, overlap_m, overlap_i);

	//internal transitions

	intTransitions(&TRANS, age_i, age_j, C_i, clu_index_i, cont_i, C_j, clu_index_j, cont_j);

	//showing transitions
	
	showTransitions(TRANS);

	//deallocating memory

	free(unique_ci);
	free(unique_cj);

	free(cont_i);
	free(cont_j);

	free(clu_index_i);
	free(clu_index_j);

	free(overlap_m);
	free(overlap_i);	

};

/*
*
*	Func: 
*		retrieveWAge(struct Clustering *);
*	Args: 
*		Array of clusterings.
*	Ret: 
*		Matrix of float, with each line representing a clustering and each cell 
*		representing an weight assigned to a sample's index.
*
*/
float **retrieveWAge(struct Clustering *C){

	float **age_w = weightAge(C);

	return age_w;

};

/*
*
*	Func: 
*		retrieveClusterings();
*	Args: 
*		None
*	Ret: 
*		An array of struct Clustering, containing all the clusterings obtained
*		by reading the csv files.
*
*/
struct Clustering *retrieveClusterings(){

	struct Clustering *C = (struct Clustering*) malloc(CLU * sizeof(struct Clustering*));

	for (int i = 0; i < CLU; ++i){
		char file[40];
		snprintf (file, sizeof (file), "Clusterings/evo_cluster_%d.csv", i);
		C[i] = readCsv(file);
	};

	return C;

};

/*
*
*	Func: 
*		readCsv(char*);
*	Args: 
*		The string path to the file containing the clustering.
*	Ret: 
*		A clustering obtained in the file, with it samples, labels and size.
*
*/
struct Clustering readCsv(char* path){

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

    return *C; 

};



/*
*
*	Func: 
*		showTransitions(struct Transitions);
*	Args: 
*		The clusters transictions between clusterings in time i and time j. (j > i)
*	Ret: 
*		None, shows to the user all the transitions obtained.  
*
*/
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
		printf("-----------------------------------------------------\n");

};






