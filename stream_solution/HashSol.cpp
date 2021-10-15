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
unordered_map<T,U> clusterWeights(vector<T> sen, vector<U> wei, clustering clus){

	unordered_map<T,U> cw;

	unordered_map<T,U> sw = makeHash<T,U>(sen, wei);

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


overlaping clusterOverlap(clustering clus, unordered_map<int, tuple<int,float>> clusE, unordered_map<int,float> cluW, vector<int> labels){

	unordered_map<int, vector<float>> intersec;

	unordered_map<int,int> lmap = useLabels(labels);
	
	for (auto c : clus){
		intersec[c.first] = vector<float>(labels.size()); //saber quantidade de grupos do agrupamento 2
		for (auto elem : c.second) {
			if(get<0>(clusE[elem]) != -1) intersec[c.first][lmap[get<0>(clusE[elem])]]+= get<1>(clusE[elem]);
		}

	}

	cout << "-----Intersection-----" << endl;
	for (auto i : intersec){
		cout << i.first << ": "; 
		for (auto x : i.second) cout << x << " ";
		cout << endl;
	}
	cout << endl;

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

unordered_map<int,int> useLabels(vector<int> labels){

	sort(labels.begin(),labels.end());

	unordered_map<int, int> lmap;

	int i = 0;
	for(auto x: labels){
		lmap[x] = i;
		i++;
	}

	return lmap;

}


template unordered_map<int, int> makeHash(vector<int>, vector<int>);
template unordered_map<int, float> makeHash(vector<int>, vector<float>);
template unordered_map<int, tuple<int, float> > makeHash(vector<int>, vector<int>, vector<float>);
template unordered_map<int,float> clusterWeights(vector<int>, vector<float>, clustering);

