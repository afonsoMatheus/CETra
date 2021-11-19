#include "TraCES.h"

int main(int argc, char const *argv[]){

	TraCES<int, int> mor(1);
	mor.configSizeLimit(0.4);
	mor.configNewIntStatistic(0.5, "Mean");
	mor.configNewIntStatistic(0.5, "Std");
	//mor.configSurvLimit(0.5);
	//mor.configSplitLimit(0.25);

	try{

	const vector<int> sensors1 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters1 = {0,0,1,1,0,0,1,1};
	const vector<float> weights1 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
		
	const vector<float> means1 = {10,20};
	const vector<float> std1 = {10,20};

	//mor.execute(sensors1, clusters1, weights1);
	mor.execute(sensors1, clusters1, weights1, {means1, std1});
	
	////////////////////////////////////////////////////////////////

	const vector<int> sensors2 = {1,2,3,4,5,6,7,8,9,10,11,12};
	const vector<int> clusters2 = {1,1,3,2,2,2,2,3,3,3,3,3};
	const vector<float> weights2 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
	
	vector<float> means2 = {10,20,10};
	vector<float> std2 = {10,20,10};

	int i = 0;
	while(i < 1){
		//mor.execute(sensors2, clusters2, weights2);
		mor.execute(sensors2, clusters2, weights2, {means2, std2});
		//means2 = {40,10,15};
		//std2 = {20,5,15};

		i++;
	}

	////////////////////////////////////////////////////////////////

	const vector<int> sensors3 = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};
	const vector<int> clusters3 = {1,1,1,1,2,2,1,1,3,3,2,2,2,2};
	const vector<float> weights3 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
	
	const vector<float> means3 = {30,20,10};
	const vector<float> std3 = {30,20,10};

	//mor.execute(sensors3, clusters3, weights3);
	mor.execute(sensors3, clusters3, weights3, {means3, std3});

	}catch(int& i){
		
		exit(i);
	}
	
	return 0;
}