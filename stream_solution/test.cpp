#include <iostream>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <bits/stdc++.h>


using namespace std;

typedef unordered_map<int, vector<int>> clustering;
typedef unordered_map<int, vector<float>> overlaping;


template <typename T, typename U >
unordered_map<T, U> makeHash(vector<T> v1, vector<U> v2){

	if(v1.size() != v2.size()){
		return unordered_map<T,U>(); 
	} 

	unordered_map<T,U> umap(v1.size());

	for (int i = 0; i < v1.size(); ++i)
		umap.insert({v1[i], v2[i]});

	/*cout << "----Hash Table----" << endl; 
	for(auto x : umap){
		cout << x.first << ": " << x.second << endl;
	}*/

	return umap;

}

clustering storeClusters(vector<int> clu, vector<int> sen){

	clustering clusters;

	//pensar no tamanho dos vetores para cada cluster
	for (int i = 0; i < sen.size(); ++i){
		clusters[clu[i]].insert(clusters[clu[i]].end(), sen[i]); 
	}


	cout << "------Clustering------" << endl;
	for (auto i : clusters){
		cout << i.first << ": "; 
		for (auto x : i.second) cout << x << " ";
		cout << endl;
	}

	return clusters;

}

template <typename T, typename U >
unordered_map<T,U> clusterWeights(vector<T> sensors, vector<U> weights, clustering clus){

	unordered_map<T,U> cw;

	unordered_map<T,U> sw = makeHash<T,U>(sensors, weights);

	for (auto i : clus){
		for(auto x : i.second) cw[i.first]+= sw[x];
	}
	
	cout << "----Cluster Weights----" << endl;
	for(auto x : cw) cout << x.first << ": " << x.second << endl;

	return cw;

}



tuple<int,int,int>* makeTuple(vector<int> sen, vector<int> clu, vector<int> wei){

	tuple<int,int,int> *t = new tuple<int,int,int>[sen.size()];

	for (int i = 0; i < sen.size(); ++i){
		t[i] = make_tuple(sen[i], clu[i], wei[i]);
		cout << get<0>(t[i]) << " " << get<1>(t[i]) << " " << get<2>(t[i]) << endl;
	}

	return t;
}

overlaping clusterOverlap(clustering clus, unordered_map<int,int> senC, unordered_map<int,float> wei, int size, unordered_map<int,float> cluW){

	unordered_map<int, vector<float>> intersec;

	for (auto i : clus){
		intersec[i.first] = vector<float>(size); //saber quantidade de grupos do agrupamento 2
		for (auto x : i.second) {
			intersec[i.first][senC[x]]+=wei[x];
		}
	}

	cout << "-----Intersection-----" << endl;
	for (auto i : intersec){
		cout << i.first << ": "; 
		for (auto x : i.second) cout << x << " ";
		cout << endl;
	}

	for (auto i : intersec){
		for (auto x = 0; x < i.second.size(); x++){
			intersec[i.first][x] =  intersec[i.first][x]/cluW[i.first];
		}
	}

	cout << "-----Overlapping-----" << endl;
	for (auto i : intersec){
		cout << i.first << ": "; 
		for (auto x : i.second) cout <<  x << " ";
		cout << endl;
	}

	return intersec;
}




int main(int argc, char const *argv[]){
	
	vector<int> sensors1 = {1,2,3,4,5,6};
	vector<int> clusters1 = {0,0,0,1,1,1};
	vector<float> weights1 = {1.0,1.0,1.0,1.0,1.0,1.0};

	//tuple<int,int,int> *t = makeTuple(sensors1, clusters1, weights1);

	/*for (int i = 0; i < 5; ++i){
		cout << get<0>(t[i]) << " " << get<1>(t[i]) << " " << get<2>(t[i]) << endl;
	}*/

	clustering clus1 = storeClusters(clusters1, sensors1);

	unordered_map<int,float> cluW = clusterWeights<int, float>(sensors1, weights1, clus1);
	
	////////////////////////////////////////////////////////////////

	cout << endl << "/////// NEXT WINDOW ///////" << endl << endl;;

	vector<int> sensors2 = {1,2,3,4,5,6};
	vector<int> clusters2 = {0,0,1,1,2,2};
	vector<float> weights2 = {0.8,0.8,0.8,0.8,0.8,0.8};

	int size = 3;

	unordered_map<int,int> senC = makeHash<int, int>(sensors2, clusters2);
	unordered_map<int,float> wei = makeHash<int, float>(sensors2, weights2);

	overlaping matrix = clusterOverlap(clus1, senC, wei, size, cluW);

	return 0;
}