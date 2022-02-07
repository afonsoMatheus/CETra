#include "/home/afonso/Documentos/Dissertação/Masters Degree/stream_solution/CETra.h"
#include "/home/afonso/Documentos/Dissertação/Masters Degree/monic/C++/Monic.cpp"
#include "/home/afonso/Documentos/Dissertação/Masters Degree/tests/ReadFile.h"
#include <chrono>
using namespace std::chrono;

int main(int argc, char const *argv[]){

	CETra<int, int> mor(1);
	mor.configSizeLimit(0.5);
	mor.configNewIntStatistic(0.5, "Sta1");

	string s1 = argv[1];
	string s2 = argv[2];

	Clustering C1 = inputClustering("/home/afonso/Documentos/Dissertação/Masters Degree/tests/" + s1);
	Clustering C2 = inputClustering("/home/afonso/Documentos/Dissertação/Masters Degree/tests/" + s2);

	try{

	auto start = high_resolution_clock::now();

	int i = 0;
	while(i < 1){

		Monic(C1, C2, 0.5, "Sta1");
		i++;
	}

	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Monic Time: " << duration.count() << endl;

	////////////////////////////////////////////////////////

	start = high_resolution_clock::now();

	mor.execute(C1.sensors, C1.clusters, C1.weights);

	i = 0;
	while(i < 1){

		mor.execute(C2.sensors, C2.clusters, C2.weights);
		i++;
	}

	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	cout << "CETra Time: " << duration.count() << endl;

	}catch(int& i){
		
		exit(i);
	}
	
	return 0;
}