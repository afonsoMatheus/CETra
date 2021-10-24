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

Transitions Monitor::extTransitions(const overlaping &matrix, const vector<int> &labels){

	unordered_map<int,int> lmap = hashLabels(labels);

	unordered_map<int, vector<int>> scands;
	set<int> tracks;

	Transitions TRANS;

	for(const auto &X : matrix){

		vector<int> split_cand;

		int split_union = -2;
		int surv_cand = -2;

		for (int Y = 0; Y < X.second.size(); Y++){

			float mcell = X.second[Y];

			if (mcell > 0.5){
				
				if(surv_cand = -2){
					surv_cand = lmap[Y];
				
				}else if(mcell > X.second[surv_cand]){

					surv_cand = lmap[Y];
				
				}else if(mcell == X.second[surv_cand]){

					//TODO: Resolver problema de empate por comp.
					//de tamanho.

				}

			}else if(mcell >= 0.25) split_cand.insert(split_cand.end(), Y);

		}

		if(surv_cand == -2 && split_cand.empty() == true){

			TRANS.insertDeath(X.first);

		}else if(split_cand.empty() == false){

			if(sumSplits(X.second, split_cand) >= 0.5){

				for(const auto x: split_cand) {
					
					TRANS.insertSplits(X.first, lmap[x]);

					tracks.insert(lmap[x]);
				};
			
			}else if(surv_cand != -2){

				scands[surv_cand].insert(scands[surv_cand].end(), X.first);
								
			}else TRANS.insertDeath(X.first);


		}else scands[surv_cand].insert(scands[surv_cand].end(), X.first);

	}

	for(const auto &y: scands){

		if(scands[y.first].size() > 1) TRANS.insertUnion(y.first, y.second);

		else TRANS.insertSurv(make_tuple(scands[y.first][0], y.first));

		tracks.insert(y.first);

	}

	set_difference(labels.begin(), labels.end(), tracks.begin(), tracks.end(),
        inserter(TRANS.allocBirths(), TRANS.allocBirths().begin()));
	
	return TRANS;
	
}

unordered_map<int,int> Monitor::hashLabels(const vector<int> &labels){

	unordered_map<int, int> umap;

	int i = 0;
	for(const auto &x: labels){
		umap[i] = x;
		i++;
	}

	return umap;

}

statistics Monitor::buildStatistics(const clustering &clusR){

	statistics cluS;

	for(const auto &x: clusR){
		cluS[x.first] = make_tuple(x.second.size(), 0, 0);

		cout << x.first << ": " << get<0>(cluS[x.first]) << endl; 
	}

	return cluS;

}

float Monitor::sumSplits(const vector<float> &overlaps, const vector<int> &split_cand){

	float sum = 0;

	for(const auto &x: split_cand){
		sum+=overlaps[x];
	}

	return sum;
}

void Monitor::showSurvs(){TRANS.showSurvs();}
void Monitor::showSplits(){TRANS.showSplits();}
void Monitor::showUnions(){TRANS.showUnions();}
void Monitor::showDeaths(){TRANS.showDeaths();}
void Monitor::showBirths(){TRANS.showBirths();}




template unordered_map<int, int> Monitor::makeHash(const vector<int>&, const vector<int>&);
template unordered_map<int, float> Monitor::makeHash(const vector<int>&, const vector<float>&);
template void Monitor::makeHash(const vector<int>&, const vector<int>&, const vector<float>&);
template void Monitor::clusterWeights(const vector<int> &, const vector<float>&);

