/* Author: Afonso Matheus                                                      */
/* Date: 2021                                                                  */
//-----------------------------------------------------------------------------//                                                                          
//                                                                             //
// Script that contains the main function for detection of transitions between //
// clusterings (See global consts in monic.c before running)                   // 
//                                                                             //
//-----------------------------------------------------------------------------//

#include "Monic.h"

void main(int argc, char const *argv[]){

	printf("Reading clusterings files...");
	struct Clustering *C = retrieveClusterings();
	printf(" Done!\n\n");


	printf("Assigning weights...");
	float **age_w = retrieveWAge(C);
	printf(" Done!\n\n");

	for (int i = 1; i < CLU; ++i){

		printf("----TRACKING TRANSITIONS IN CLUSTERINGS T_%d -> T_%d----\n\n", i-1, i);

		monic(C[i-1], C[i], age_w[i-1], age_w[i]);

		printf("\n");
	};
	
};