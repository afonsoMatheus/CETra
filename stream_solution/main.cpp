#include "Monitor.h"

int main(int argc, char const *argv[]){

	Monitor<int, int> mor(3,{0.5,0.2});
	mor.setSizeLimit(0.2);
	mor.setStaNames({"Mean","Std"});
	
	const vector<int> sensors1 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters1 = {1,1,1,1,2,2,2,2};
	const vector<float> weights1 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
	const vector<int> labels1 = {1,2};
		
	const vector<float> means1 = {10,20};
	const vector<float> std1 = {10,20};

	//mor.execute(sensors1, clusters1, weights1, labels1);
	mor.execute(sensors1, clusters1, weights1, labels1, {means1, std1});
	
	////////////////////////////////////////////////////////////////

	const vector<int> sensors2 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters2 = {4,4,4,4,5,5,6,6};
	const vector<float> weights2 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
	const vector<int> labels2 = {4,5,6}; //precisa ser ordenado
	
	const vector<float> means2 = {20,10,20};
	const vector<float> std2 = {20,10,20};

	int i = 0;
	while(i < 4){
		//mor.execute(sensors2, clusters2, weights2, labels2);
		mor.execute(sensors2, clusters2, weights2, labels2,{means2, std2});
		i++;
	}

	//mor.showStatistics();

	////////////////////////////////////////////////////////////////


	return 0;
}