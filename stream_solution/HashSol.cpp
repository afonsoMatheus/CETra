#include "HashSol.h"

template <typename T, typename U >
unordered_map<T, U> makeHash(vector<T> v1, vector<U> v2){

	if(v1.size() != v2.size()){
		return unordered_map<T,U>(); 
	} 

	unordered_map<T,U> umap(v1.size());

	for (int i = 0; i < v1.size(); ++i)
		umap.insert({v1[i], v2[i]});

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

	return umap;

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

template unordered_map<int, int> makeHash(vector<int> v1, vector<int> v2);
template unordered_map<int, float> makeHash(vector<int> v1, vector<float> v2);
template unordered_map<int, tuple<int, float> > makeHash(vector<int> v1, vector<int> v2, vector<float> v3);
template unordered_map<int,float> clusterWeights(vector<int> sensors, vector<float> weights, clustering clus);

