#include "Monitor.h"

int main(int argc, char const *argv[]){
	
	const vector<int> sensors1 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters1 = {1,1,1,1,2,2,3,3};
	const vector<float> weights1 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};

	Monitor<int,int,float> mor;

	mor.relClustering(sensors1, clusters1, weights1);
	
	////////////////////////////////////////////////////////////////

	cout << endl << "///// NEXT WINDOW /////" << endl << endl;;

	const vector<int> sensors2 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters2 = {4,4,5,5,6,6,6,6};
	const vector<float> weights2 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
	vector<int> labels = {4,6,5}; //precisa ser ordenado

	mor.checkEvolution(sensors2, clusters2, weights2, labels);

	////////////////////////////////////////////////////////////////

	cout << "---EXT TRANSITIONS---" << endl << endl; 

	mor.showSurvs();

	mor.showSplits();

	mor.showUnions();

	mor.showDeaths();

	mor.showBirths();

	return 0;
}