#include "HashSol.h"

template <typename T, typename U >
unordered_map<T, U> makeHash(const vector<T> &v1, const vector<U> &v2){

	if(v1.size() != v2.size()){
		return unordered_map<T,U>(); 
	} 

	unordered_map<T,U> umap(v1.size());

	for (int i = 0; i < v1.size(); ++i)
		umap.insert({v1[i], v2[i]});

	return umap;

}

template <typename T, typename U , typename V>
unordered_map<T, tuple<U, V> > makeHash(const vector<T> &v1, const vector<U> &v2, const vector<V> &v3){

	if(v1.size() != v2.size() or v1.size() != v3.size()){
		return unordered_map<T, tuple<U, V> >(); 
	} 

	unordered_map<T, tuple < U, V >> umap(v1.size());

	for (int i = 0; i < v1.size(); ++i)
		umap.insert({v1[i], make_tuple(v2[i],v3[i])});

	return umap;

}

template <typename T, typename U >
unordered_map<T,U> clusterWeights(const vector<T> &sen, const vector<U> &wei, const clustering &clus) {

	unordered_map<T,U> cw;

	unordered_map<T,U> sw = makeHash<T,U>(sen, wei);

	for (const auto &i : clus){
		for(auto x : i.second) cw[i.first]+= sw[x];
	}
	
	cout << "----Cluster Weights----" << endl;
	for(const auto &x : cw) cout << x.first << ": " << x.second << endl;

	return cw;

}

clustering storeClusters(const vector<int> &clu, const vector<int> &sen){

	clustering clusters;

	//pensar no tamanho dos vetores para cada cluster
	for (int i = 0; i < sen.size(); ++i){
		if(clu[i] != -1) clusters[clu[i]].insert(clusters[clu[i]].end(), sen[i]); 
	}


	cout << "------Clustering------" << endl;
	for (const auto &i : clusters){
		cout << i.first << ": "; 
		for (const auto &x : i.second) cout << x << " ";
		cout << endl;
	}

	return clusters;

}


overlaping clusterOverlap(const clustering &clus, unordered_map<int, tuple<int,float>> &clusE, unordered_map<int,float> &cluW, vector<int> &labels){

	unordered_map<int, vector<float>> intersec;

	unordered_map<int,int> lmap = useLabels(labels);
	
	for (auto &c : clus){
		intersec[c.first] = vector<float>(labels.size()); //saber quantidade de grupos do agrupamento 2
		for (auto &elem : c.second) {
			if(get<0>(clusE[elem]) != -1) intersec[c.first][lmap[get<0>(clusE[elem])]]+= get<1>(clusE[elem]);
		}

	}

	cout << "-----Intersection-----" << endl;
	for (const auto &i : intersec){
		cout << i.first << ": "; 
		for (const auto &x : i.second) cout << x << " ";
		cout << endl;
	}
	cout << endl;

	for (const auto &i : intersec){
		for (auto x = 0; x < i.second.size(); x++){
			intersec[i.first][x] =  intersec[i.first][x]/cluW[i.first];
		}
	}

	cout << "-----Overlapping-----" << endl;
	for (const auto &i : intersec){
		cout << i.first << ": "; 
		for (const auto &x : i.second) cout <<  x << " ";
		cout << endl;
	}
	cout << endl;

	return intersec;
}

unordered_map<int,int> useLabels(vector<int> &labels){

	sort(labels.begin(),labels.end());

	unordered_map<int, int> lmap;

	int i = 0;
	for(const auto &x: labels){
		lmap[x] = i;
		i++;
	}

	return lmap;

}


template unordered_map<int, int> makeHash(const vector<int>&, const vector<int>&);
template unordered_map<int, float> makeHash(const vector<int>&, const vector<float>&);
template unordered_map<int, tuple<int, float> > makeHash(const vector<int>&, const vector<int>&, const vector<float>&);
template unordered_map<int,float> clusterWeights(const vector<int> &, const vector<float>&, const clustering&);

