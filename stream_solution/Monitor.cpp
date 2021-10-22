#include "Monitor.h"

template <typename T, typename U >
unordered_map<T, U> Monitor::makeHash(const vector<T> &v1, const vector<U> &v2){

	if(v1.size() != v2.size()){
		return unordered_map<T,U>(); 
	} 

	unordered_map<T,U> umap(v1.size());

	for (int i = 0; i < v1.size(); ++i)
		umap.insert({v1[i], v2[i]});

	return umap;

}

template <typename T, typename U , typename V>
void Monitor::makeHash(const vector<T> &v1, const vector<U> &v2, const vector<V> &v3){

	if(v1.size() != v2.size() or v1.size() != v3.size()){
		
	} 

	//unordered_map<T, tuple < U, V >> umap(v1.size());

	for (int i = 0; i < v1.size(); ++i)
		clusE.insert({v1[i], make_tuple(v2[i],v3[i])});

}

template <typename T, typename U >
void Monitor::clusterWeights(const vector<T> &sen, const vector<U> &wei) {

	unordered_map<T,U> sw = makeHash<T,U>(sen, wei);

	for (const auto &i : clusR){
		for(auto x : i.second) cluW[i.first]+= sw[x];
	}
	
	cout << "----Cluster Weights----" << endl;
	for(const auto &x : cluW) cout << x.first << ": " << x.second << endl;


}

void Monitor::storeClusters(const vector<int> &clu, const vector<int> &sen){

	//pensar no tamanho dos vetores para cada cluster
	for (int i = 0; i < sen.size(); ++i){
		if(clu[i] != -1) clusR[clu[i]].insert(clusR[clu[i]].end(), sen[i]); 
	}


	cout << "------Clustering------" << endl;
	for (const auto &i : clusR){
		cout << i.first << ": "; 
		for (const auto &x : i.second) cout << x << " ";
		cout << endl;
	}

}


void Monitor::clusterOverlap(){

	unordered_map<int,int> lmap = useLabels();
	
	for (auto &c : clusR){
		matrix[c.first] = vector<float>(labels.size()); //saber quantidade de grupos do agrupamento 2
		for (auto &elem : c.second) {
			if(get<0>(clusE[elem]) != -1) matrix[c.first][lmap[get<0>(clusE[elem])]]+= get<1>(clusE[elem]);
		}

	}

	cout << "-----Intersection-----" << endl;
	for (const auto &i : matrix){
		cout << i.first << ": "; 
		for (const auto &x : i.second) cout << x << " ";
		cout << endl;
	}
	cout << endl;

	for (const auto &i : matrix){
		for (auto x = 0; x < i.second.size(); x++){
			matrix[i.first][x] =  matrix[i.first][x]/cluW[i.first];
		}
	}

	cout << "-----Overlapping-----" << endl;
	for (const auto &i : matrix){
		cout << i.first << ": "; 
		for (const auto &x : i.second) cout <<  x << " ";
		cout << endl;
	}
	cout << endl;

}

unordered_map<int,int> Monitor::useLabels(){

	unordered_map<int, int> lmap;

	int i = 0;
	for(const auto &x: labels){
		lmap[x] = i;
		i++;
	}

	return lmap;

}

void Monitor::storeLabels(vector<int> lab){

	labels = lab;

	sort(labels.begin(),labels.end());

}

void Monitor::Run(){

	clusterOverlap();

	TRANS = extTransitions(matrix, labels);
}



template unordered_map<int, int> Monitor::makeHash(const vector<int>&, const vector<int>&);
template unordered_map<int, float> Monitor::makeHash(const vector<int>&, const vector<float>&);
template void Monitor::makeHash(const vector<int>&, const vector<int>&, const vector<float>&);
template void Monitor::clusterWeights(const vector<int> &, const vector<float>&);

