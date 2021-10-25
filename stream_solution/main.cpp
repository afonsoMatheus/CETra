#include "Monitor.h"

int main(int argc, char const *argv[]){
	
	const vector<int> sensors1 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters1 = {1,1,1,1,2,2,3,3};
	const vector<float> weights1 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
	const vector<int> labels1 = {1,2,3};

	const vector<float> means1 = {10,5,20};
	const vector<float> std1 = {1, 1.5, 2};
 
	Monitor<int, int, float> mor;

	mor.execute(sensors1, clusters1, weights1, labels1, {means1});
	
	////////////////////////////////////////////////////////////////

	cout << endl << "///// NEXT WINDOW /////" << endl << endl;

	const vector<int> sensors2 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters2 = {4,4,5,5,6,6,6,6};
	const vector<float> weights2 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
	vector<int> labels2 = {4,6,5}; //precisa ser ordenado

	mor.execute(sensors2, clusters2, weights2, labels2,{means1, std1});

	////////////////////////////////////////////////////////////////

	cout << "---EXT TRANSITIONS---" << endl << endl; 

	mor.showSurvs();

	mor.showSplits();

	mor.showUnions();

	mor.showDeaths();

	mor.showBirths();

	return 0;
}