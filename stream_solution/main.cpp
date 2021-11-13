#include "Monitor.h"

int main(int argc, char const *argv[]){

	Monitor<int, int> mor(3,{0.4,0.4});
	mor.configSizeLimit(0.5);
	mor.configStaNames({"Mean", "Std"});

	try{
	
	const vector<int> sensors1 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters1 = {0,0,0,0,2,2,2,2};
	const vector<float> weights1 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
	const vector<int> labels1 = {0,2};
		
	const vector<float> means1 = {10,20};
	const vector<float> std1 = {10,20};

	//mor.execute(sensors1, clusters1, weights1, labels1);
	mor.execute(sensors1, clusters1, weights1, labels1, {means1, std1});
	
	////////////////////////////////////////////////////////////////

	const vector<int> sensors2 = {1,2,3,4,6,12,11,9,10};
	const vector<int> clusters2 = {0,0,1,1,2,2,2,2,2};
	const vector<float> weights2 = {1.0,1.0,1.0,0.0,1.0,1.0,1.0,1.0,1.0};
	const vector<int> labels2 = {0,1,2}; //precisa ser ordenado
	
	vector<float> means2 = {20,10,15};
	vector<float> std2 = {20,10,15};

	int i = 0;
	while(i < 4){
		//mor.execute(sensors2, clusters2, weights2, labels2);
		mor.execute(sensors2, clusters2, weights2, labels2,{means2, std2});
		means2 = {40,10,15};
		std2 = {20,5,15};

		i++;
	}

	//mor.showStatistics();

	////////////////////////////////////////////////////////////////

	const vector<int> sensors3 = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};
	const vector<int> clusters3 = {1,1,1,1,2,2,1,1,3,3,2,2,2,2};
	const vector<float> weights3 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
	const vector<int> labels3 = {1,2,3}; //precisa ser ordenado
	
	const vector<float> means3 = {30,20,10};
	const vector<float> std3 = {30,20,10};

	//mor.execute(sensors3, clusters3, weights3, labels3);
	mor.execute(sensors3, clusters3, weights3, labels3,{means3, std3});

	}catch(int& i){
		
		exit(i);
	}
	
	return 0;
}