#include "/home/afonso/Documentos/Dissertação/Masters Degree/stream_solution/CETra.h"
#include "/home/afonso/Documentos/Dissertação/Masters Degree/monic/C++/Monic.cpp"
#include "/home/afonso/Documentos/Dissertação/Masters Degree/tests/ReadFile.h"
#include <chrono>
using namespace std::chrono;

int main(int argc, char const *argv[]){

	string s1 = argv[1];
	string s2 = argv[2];

	Clustering C1 = inputClustering("/home/afonso/Documentos/Dissertação/Masters Degree/tests/basic_test/" + s1);
	Clustering C2 = inputClustering("/home/afonso/Documentos/Dissertação/Masters Degree/tests/basic_test/" + s2);

	try{

	vector<float> lim = {0.3, 0.3, 0.3, 0.3, 0.3};
	vector<string> nam = {"Size", "Sta2", "Sta3", "Sta4", "Sta5"};

	//auto start = high_resolution_clock::now();

	Transitions<int> TRANS_M;

	int i = 0;
	while(i < 1){

		TRANS_M = Monic(C1, C2, lim, nam);
		i++;
	}

	//auto stop = high_resolution_clock::now();
	//auto duration = duration_cast<microseconds>(stop - start);
	//cout << "Monic Time: " << duration.count() << endl;

	TRANS_M.showSurvs();
	TRANS_M.showTransitions();

	TRANS_M.exportTrans("/home/afonso/Documentos/Dissertação/Masters Degree/tests/basic_test/output/sankey_Monic.txt");

	////////////////////////////////////////////////////////

	CETra<int, int> mor(1);
	mor.configSizeLimit(0.4);
	//mor.configNewIntStatistic(0.3, "Sta1");
	mor.configNewIntStatistic(0.4, "Sta2");
	mor.configNewIntStatistic(0.4, "Sta3");
	mor.configNewIntStatistic(0.4, "Sta4");
	mor.configNewIntStatistic(0.4, "Sta5");

	//start = high_resolution_clock::now();

	mor.execute(C1.sensors, C1.clusters, C1.weights, {C1.inter[1], C1.inter[2], C1.inter[3], C1.inter[4]});

	Transitions<int> TRANS_C;

	i = 0;
	while(i < 1){
		
		TRANS_C = mor.execute(C2.sensors, C2.clusters, C2.weights, {C2.inter[1], C2.inter[2], C2.inter[3], C2.inter[4]});
		i++;
	}

	//stop = high_resolution_clock::now();
	//duration = duration_cast<microseconds>(stop - start);
	//cout << "CETra Time: " << duration.count() << endl;

	TRANS_C.showSurvs();
	TRANS_C.showTransitions();
	//TRANS_C.showIntMatrix();

	TRANS_C.exportTrans("/home/afonso/Documentos/Dissertação/Masters Degree/tests/basic_test/output/sankey_CETra.txt");

	}catch(int& i){
		
		exit(i);
	}
	
	return 0;
}