#include "/home/afonso/Documentos/Dissertação/Masters Degree/stream_solution/CETra.h"
#include "/home/afonso/Documentos/Dissertação/Masters Degree/monic/C++/Monic.cpp"
#include "/home/afonso/Documentos/Dissertação/Masters Degree/tests/ReadFile.h"
#include <chrono>
#include <iostream>
#include <fstream>
using namespace std::chrono;

int main(int argc, char const *argv[]){

	int clus = 20;
	int win = 10;
	int tests = 5;

	vector<Clustering> C;
	for(auto i = 0; i < clus; i++){
		C.push_back(inputClustering("/home/afonso/Documentos/Dissertação/Masters Degree/tests/stress_test/c" + to_string(i)));
	}

	////////////////////////////////////////////////////////

	vector<float> monic_time;

	try{

	vector<float> lim = {0.3};
	vector<string> nam = {"Sta1"};

	for(auto i = 0; i < clus; i++){

		float mean = 0;

		for(auto x = 0; x < tests; x++){

			auto start = high_resolution_clock::now();

			for(auto j = 0; j < win; j++){
				
				Monic(C[i], C[i], lim, nam);
			}

			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "Monic Time: " << duration.count() << endl;

			mean = mean + duration.count();
		}

		monic_time.push_back(mean/tests);

	}

	cout << "******************************"<< endl << endl;

	////////////////////////////////////////////////////////

	CETra<int, int> mor(1);
	mor.configNewIntStatistic(0.3, "Sta1");

	vector<float> cetra_time;

	for(auto i = 0; i < clus; i++){

		float mean = 0;

		for(auto x = 0; x < tests; x++){

			auto start = high_resolution_clock::now();

			for(auto j = 0; j < win; j++){
				mor.execute(C[i].sensors, C[i].clusters, C[i].weights);
			}

			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "CETra Time: " << duration.count() << endl;

			mean = mean + duration.count();
			
		}

		cetra_time.push_back(mean/tests);
	}

	ofstream output("/home/afonso/Documentos/Dissertação/Masters Degree/tests/stress_test/output/graph");

	for(const auto &x: monic_time){ 
		output << x << " ";
	}	
	output << endl;

	for(const auto &x: cetra_time){ 
		output << x << " ";
	}	
	output << endl;

	for(auto x = 0; x < clus; x++){ 
		output << C[x].sensors.size() << " ";
	}	
	output << endl;

	}catch(int& i){
		
		exit(i);
	}
	
	return 0;
}