#include "/home/afonso/Documentos/Dissertação/Masters Degree/stream_solution/CETra.h"
#include "/home/afonso/Documentos/Dissertação/Masters Degree/monic/C++/Monic.cpp"
#include "ReadFile.h"
#include <chrono>
using namespace std::chrono;

int main(int argc, char const *argv[]){

	CETra<int, int> mor(1);
	mor.configSizeLimit(0.5);
	mor.configNewIntStatistic(0.5, "Mean");
	mor.configNewIntStatistic(0.5, "Std");

	Clustering C1 = inputClustering("/home/afonso/Documentos/Dissertação/Masters Degree/tests/basic_test/iFCDS3");
	Clustering C2 = inputClustering("/home/afonso/Documentos/Dissertação/Masters Degree/tests/basic_test/iFCDS4");

	try{

	auto start = high_resolution_clock::now();

	int i = 0;
	/*while(i < 1000){

		Monic(C1, C2);
		i++;

	}*/

	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);
	cout << duration.count() << endl;

	////////////////////////////////////////////////////////

	start = high_resolution_clock::now();

	mor.execute(C1.sensors, C1.clusters, C1.weights);

	Transitions<int> TRANS;

	//cout << C2.sensors.size() << " " << C2.clusters.size()<< " " << C2.weights.size()<< " " << endl;

	i = 0;
	while(i < 1){
		
		TRANS = mor.execute(C2.sensors, C2.clusters, C2.weights);
		i++;
	}

	TRANS.exportTrans("/home/afonso/Documentos/Dissertação/Masters Degree/tests/output/sankey.txt");

	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	cout << "Time: " << duration.count() << endl;

	}catch(int& i){
		
		exit(i);
	}
	
	return 0;
}