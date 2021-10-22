#include "ExternalMon.h"
#include "Transitions.h"

int main(int argc, char const *argv[]){
	
	const vector<int> sensors1 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters1 = {1,1,1,1,1,3,4,4};
	const vector<float> weights1 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};

	clustering clusR = storeClusters(clusters1, sensors1);

	//statistics cluS = buildStatistics(clusR);

	unordered_map<int,float> cluW = clusterWeights<int, float>(sensors1, weights1, clusR);
	
	////////////////////////////////////////////////////////////////

	cout << endl << "///// NEXT WINDOW /////" << endl << endl;;

	const vector<int> sensors2 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters2 = {2,3,5,6,7,4,4,4};
	const vector<float> weights2 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
	vector<int> labels = {2,4,3,5,6,7}; //precisa ser ordenado
	
	unordered_map<int, tuple<int, float>> clusE = makeHash<int, int, float>(sensors2, clusters2, weights2);

	overlaping matrix = clusterOverlap(clusR, clusE, cluW, labels);

	////////////////////////////////////////////////////////////////

	Transitions TRANS = extTransitions(matrix, labels);

	cout << "---EXT TRANSITIONS---" << endl << endl; 

	TRANS.showSurvs();

	TRANS.showSplits();

	TRANS.showUnions();

	TRANS.showDeaths();

	TRANS.showBirths();

	return 0;
}