#include "Monitor.h"

int main(int argc, char const *argv[]){

	Monitor<int, int> mor(3,{0.5,0.5});
	mor.setSizeLimit(0.3);
	
	const vector<int> sensors1 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters1 = {1,1,1,1,2,2,2,2};
	const vector<float> weights1 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
	const vector<int> labels1 = {1,2};
		
	int_statistic means1({10,20});
	int_statistic std1({10,20});

	//mor.execute(sensors1, clusters1, weights1, labels1);
	mor.execute(sensors1, clusters1, weights1, labels1, {means1, std1});
	
	////////////////////////////////////////////////////////////////

	cout << endl << "///// NEXT WINDOW /////" << endl << endl;

	const vector<int> sensors2 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters2 = {6,6,6,6,6,4,4,4};
	const vector<float> weights2 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
	vector<int> labels2 = {4,6}; //precisa ser ordenado
	
	int_statistic means2({20});
	int_statistic std2({20,10});

	int i = 0;
	while(i < 1){
		//mor.execute(sensors2, clusters2, weights2, labels2);
		mor.execute(sensors2, clusters2, weights2, labels2,{means2, std2});
		i++;
	}

	//mor.showStatistics();

	////////////////////////////////////////////////////////////////


	return 0;
}