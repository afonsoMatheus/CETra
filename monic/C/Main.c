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

	struct Clustering *C = retrieveClusterings();
	float **age_w = retrieveWAge(C);

	for (int i = 1; i < CLU; ++i){

		monic(C[i-1], C[i], age_w[i-1], age_w[i]);
	};
	
};