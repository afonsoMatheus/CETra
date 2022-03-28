#ifndef READFILE_H
#define READFILE_H

#include <fstream>
#include <sstream>
#include <map>
#include <string>

struct Clustering{

	vector<int> sensors;
	vector<int> clusters;
	vector<float> weights;
	map<int, vector<float>> inter;

};

Clustering inputClustering(const string &path){

	ifstream MyReadFile(path);
	string line;

	Clustering C;

	getline (MyReadFile, line);
	stringstream ss(line);
	string word;
    while (ss >> word) {
        C.sensors.push_back(stoi(word));
    }

	/*for(const auto &x: C.sensors)
		cout << x << " ";
	cout << endl << endl;*/

	getline (MyReadFile, line);
	stringstream s2(line);
    while (s2 >> word) {
        C.clusters.push_back(stoi(word));
    }

    /*for(const auto &x: C.clusters)
		cout << x << " ";
	cout << endl << endl;
*/
	getline (MyReadFile, line);
	stringstream s3(line);
    while (s3 >> word) {
        C.weights.push_back(stoi(word));
    }

    /*for(const auto &x: C.weights)
		cout << x << " ";
	cout << endl << endl;
*/
	int i = 0;
	while (getline (MyReadFile, line)) {
		stringstream ss(line);
	    C.inter[i] = {};
	    while (ss >> word) {
	        C.inter[i].push_back(stof(word));
	    }
	    i++;
	}

	/*for(const auto &x: C.inter){
		for(const auto &y: x.second){
			cout << y << " ";
		}
		cout << endl << endl;	
	}*/

	MyReadFile.close();

	return C;

}

#endif