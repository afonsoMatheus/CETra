#include <iostream>
#include <unordered_map>
#include <vector>
#include <tuple>

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

template <typename T, typename U , typename V>
unordered_map<T, tuple<U, V> > makeHash(vector<T> v1, vector<U> v2, vector<V> v3){

	if(v1.size() != v2.size() or v1.size() != v3.size()){
		return unordered_map<T, tuple<U, V> >(); 
	} 

	unordered_map<T, tuple < U, V >> umap(v1.size());

	for (int i = 0; i < v1.size(); ++i)
		umap.insert({v1[i], make_tuple(v2[i],v3[i])});

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

overlaping clusterOverlap(clustering clus, unordered_map<int, tuple<int,float>> clusE, unordered_map<int,float> cluW, int size){

	unordered_map<int, vector<float>> intersec;

	for (auto i : clus){
		intersec[i.first] = vector<float>(size); //saber quantidade de grupos do agrupamento 2
		for (auto x : i.second) {
			intersec[i.first][get<0>(clusE[x])]+= get<1>(clusE[x]);
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
	cout << endl;

	return intersec;
}

int main(int argc, char const *argv[]){
	
	vector<int> sensors1 = {1,2,3,4,5,6};
	vector<int> clusters1 = {0,0,0,1,1,1};
	vector<float> weights1 = {1.0,1.0,1.0,1.0,1.0,1.0};

	clustering clus1 = storeClusters(clusters1, sensors1);

	unordered_map<int,float> cluW = clusterWeights<int, float>(sensors1, weights1, clus1);
	
	////////////////////////////////////////////////////////////////

	cout << endl << "///// NEXT WINDOW /////" << endl << endl;;

	vector<int> sensors2 = {1,2,3,4,5,6};
	vector<int> clusters2 = {0,0,1,1,2,2};
	vector<float> weights2 = {0.8,0.8,0.8,0.8,0.8,0.8};
	int size = 3;

	unordered_map<int, tuple<int, float>> clusE = makeHash<int, int, float>(sensors2, clusters2, weights2);

	overlaping matrix = clusterOverlap(clus1, clusE, cluW, size);

	return 0;
}