#include "Monitor.h"

int main(int argc, char const *argv[]){

	Monitor<int, int, float> mor(5);
	
	const vector<int> sensors1 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters1 = {1,1,1,1,2,2,2,2};
	const vector<float> weights1 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
	const vector<int> labels1 = {1,2};
	
	const vector<float> sizes1 = {4,4};
	const vector<float> means1 = {4,2};
	const vector<float> std1 = {1,2.5};

	mor.execute(sensors1, clusters1, weights1, labels1, {sizes1, means1, std1});
	
	////////////////////////////////////////////////////////////////

	cout << endl << "///// NEXT WINDOW /////" << endl << endl;

	const vector<int> sensors2 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters2 = {4,4,4,4,6,6,6,6};
	const vector<float> weights2 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
	vector<int> labels2 = {4,6}; //precisa ser ordenado

	const vector<float> sizes2 = {2,4};
	const vector<float> means2 = {12,13};
	const vector<float> std2 = {2,3};

	mor.execute(sensors2, clusters2, weights2, labels2,{sizes2, means2, std2});

	//mor.showStatistics();

	////////////////////////////////////////////////////////////////


	return 0;
}