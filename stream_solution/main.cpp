#include "Monitor.h"

int main(int argc, char const *argv[]){
	
	const vector<int> sensors1 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters1 = {1,1,1,1,2,2,3,3};
	const vector<float> weights1 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};

	Monitor mor;

	mor.storeClusters(clusters1, sensors1);

	//statistics cluS = buildStatistics(clusR);

	mor.clusterWeights<int, float>(sensors1, weights1);
	
	////////////////////////////////////////////////////////////////

	cout << endl << "///// NEXT WINDOW /////" << endl << endl;;

	const vector<int> sensors2 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters2 = {2,2,2,2,4,4,5,5};
	const vector<float> weights2 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
	vector<int> labels = {4,2,5}; //precisa ser ordenado
	
	mor.storeLabels(labels);

	mor.makeHash<int, int, float>(sensors2, clusters2, weights2);

	mor.Run();

	////////////////////////////////////////////////////////////////

	cout << "---EXT TRANSITIONS---" << endl << endl; 

	mor.TRANS.showSurvs();

	mor.TRANS.showSplits();

	mor.TRANS.showUnions();

	mor.TRANS.showDeaths();

	mor.TRANS.showBirths();

	return 0;
}