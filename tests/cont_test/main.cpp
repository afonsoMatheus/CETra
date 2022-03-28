#include "/home/afonso/Documentos/Dissertação/Masters Degree/stream_solution/CETra.h"
#include "/home/afonso/Documentos/Dissertação/Masters Degree/monic/C++/Monic.cpp"
#include "/home/afonso/Documentos/Dissertação/Masters Degree/tests/ReadFile.h"
#include <chrono>
using namespace std::chrono;

int main(int argc, char const *argv[]){

	int win = 20;

	vector<Clustering> C;
	for(auto i = 0; i < win; i++){
		C.push_back(inputClustering("/home/afonso/Documentos/Dissertação/Masters Degree/tests/cont_test/c" + to_string(i)));
	}

	////////////////////////////////////////////////////////

	try{

	vector<float> lim = {0.3};
	vector<string> nam = {"Sta1"};

	auto start = high_resolution_clock::now();

	for(auto i = 1; i < win; i++){
		
		Transitions<int> TRANS;

		TRANS = Monic(C[i-1], C[i], lim, nam);

		if(!TRANS.getInterC().empty()){
			TRANS.showTransitions();
			cout << "Detected in " << i+1 << endl << endl;
			cout << "///////////////////////"<< endl << endl;
		}
	}

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Monic Time: " << duration.count() << endl << endl;

	cout << "******************************"<< endl << endl;

	////////////////////////////////////////////////////////

	CETra<int, int> mor(1);
	mor.configNewIntStatistic(0.3, "Sta1");

	start = high_resolution_clock::now();

	for(auto i = 0; i < win; i++){
		
		Transitions<int> TRANS;

		TRANS = mor.execute(C[i].sensors, C[i].clusters, C[i].weights, {C[i].inter[0]});

		if(!TRANS.getInterC().empty()){
			cout << "Detected in " << i+1 << endl << endl;
			cout << "///////////////////////"<< endl << endl;
		}

	}

	//TRANS.exportTrans("/home/afonso/Documentos/Dissertação/Masters Degree/tests/output/sankey.txt");

	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	
	cout << "CETra Time: " << duration.count() << endl;

	}catch(int& i){
		
		exit(i);
	}
	
	return 0;
}