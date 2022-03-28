#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <unordered_map>
//#include "/home/afonso/Documentos/Dissertação/Masters Degree/tests/ReadFile.h"

using namespace std;

unordered_map<unsigned int, vector<float>> overlapMatrix(const Clustering &C1, const Clustering &C2, const vector<unsigned int> &aux){

	unordered_map<unsigned int, vector<float>> om;

	unordered_map<unsigned int,float> wei;

	unordered_map<unsigned int,int> lmap;

	unsigned int a = 0;
	for(const auto &x: aux){
		lmap[x] = a;
		a++;
	}

	for(auto i = 0; i < C1.sensors.size(); i++){

		if(om.find(C1.clusters[i]) == om.end())
			om[C1.clusters[i]] = vector<float>(aux.size());
		wei[C1.clusters[i]] = wei[C1.clusters[i]] + C1.weights[i];

		for(auto j = 0; j < C2.sensors.size(); j++){

			if(C1.sensors[i] == C2.sensors[j]){

				om[C1.clusters[i]][lmap[C2.clusters[j]]] = om[C1.clusters[i]][lmap[C2.clusters[j]]] + C2.weights[j];

			}
		}

	}

	for(const auto &i: om){
		for (auto x = 0; x < i.second.size(); x++){
			om[i.first][x] = om[i.first][x]/wei[i.first];
		}
	}

	return om;
}

template <typename C>
float sumSplits(const vector<float> &overlaps, const vector<C> &split_cand){

	float sum = 0.0;

	for(const auto &x: split_cand){
		sum+=overlaps[x];
	}

	return sum;
}

template <typename C>
Transitions<C> extTransitions(const unordered_map<C, vector<float>> &matrix, const vector<C> &aux){

	Transitions<C> TRANS;

	unordered_map<int,C> lmap;
	int a = 0;
	for(const auto &x: aux){
		lmap[a] = x;
		a++;
	}

	unordered_map<C, vector<C>> scands;
	set<C> tracks;

	for(const auto &X : matrix){

		vector<C> split_cand;

		C split_union = (C) -2;
		C surv_cand = (C) -2;

		for (int Y = 0; Y < X.second.size(); Y++){

			float mcell = X.second[Y];

			if (mcell >= 0.5){
				
				if(surv_cand = (C) -2){
					surv_cand = lmap[Y];
				
				}else if(mcell > X.second[surv_cand]){

					surv_cand = lmap[Y];
				
				}else if(mcell == X.second[surv_cand]){

					/*if(clusR[surv_cand].size() < clusR[lmap[Y]].size()){
						surv_cand = lmap[Y];
					}*/
				}

			}else if(mcell >= 0.25) {
				
				split_cand.insert(split_cand.end(), Y);
			}

		}

		if(surv_cand == (C) -2 && split_cand.empty() == true){

			TRANS.insertDeath(X.first);

			//checkFailDeath(X.first, TRANS);

		}else if(split_cand.empty() == false){

			float split_weis = sumSplits(X.second, split_cand);

			if(split_weis >= 0.5){

				for(const auto x: split_cand) {
					
					TRANS.insertSplits(X.first, lmap[x]);

					tracks.insert(lmap[x]);
				}
			
			}else if(surv_cand != (C) -2){

				scands[surv_cand].insert(scands[surv_cand].end(), X.first);
								
			}else TRANS.insertDeath(X.first);


		}else scands[surv_cand].insert(scands[surv_cand].end(), X.first);

	}

	for(const auto &y: scands){

		if(scands[y.first].size() > 1) TRANS.insertUnion(y.first, y.second);

		else TRANS.insertSurv(scands[y.first][0], y.first);;

		tracks.insert(y.first);

	}

	set_difference(aux.begin(), aux.end(), tracks.begin(), tracks.end(),
        inserter(TRANS.allocBirths(), TRANS.allocBirths().begin()));

	//checkNewBirth(TRANS);

	return TRANS;

}

template <typename C>
void intTransitions(Transitions<C>& TRANS, unordered_map<unsigned int, vector<float>>& sta1, unordered_map<unsigned int, vector<float>>& sta2, vector<float>& limits){

	unordered_map<unsigned int, vector<float>> inter;

	for(const auto &x: TRANS.getSurvs()){
		for(int i = 0; i < sta1[x.first].size(); i++){
			if(i < sta2[x.second].size())
				inter[x.first].insert(inter[x.first].end(), sta2[x.second][i]/sta1[x.first][i]);
		}
	}

	checkIntTrans(TRANS, limits, inter);
}

template <typename C>
void checkIntTrans(Transitions<C>& TRANS, vector<float>& limits, unordered_map<unsigned int, vector<float>> &inter){

	for(const auto &clu: inter){
		for(auto i = 0; i < clu.second.size(); i++){
			if(fabs(clu.second[i]-1) > limits[i]){
				TRANS.insertInterC(clu.first, i);
			}
		}
	}	

}

Transitions<unsigned int> Monic(const Clustering &C1, const Clustering &C2, vector<float>& limits, vector<string>& names){

	vector<unsigned int> aux_c1 = C1.clusters; 
	sort( aux_c1.begin(), aux_c1.end());
	aux_c1.erase( unique( aux_c1.begin(), aux_c1.end() ), aux_c1.end());

	unordered_map<unsigned int, vector<float>> staC1;
	for(const auto &x: C1.inter){
		int i = 0;
		for(const auto &y: aux_c1){
			staC1[y].push_back(x.second[i]);
			i++;
		}
	}

	vector<unsigned int> aux = C2.clusters; 
	sort( aux.begin(), aux.end() );
	aux.erase( unique( aux.begin(), aux.end() ), aux.end());

	unordered_map<unsigned int, vector<float>> staC2;
	for(const auto &x: C2.inter){
		int i = 0;
		for(const auto &y: aux){
			staC2[y].push_back(x.second[i]);
			i++;
		}
	}

	unordered_map<unsigned int, vector<float>> mat = overlapMatrix(C1, C2, aux);

	Transitions<unsigned int> TRANS = extTransitions<unsigned int>(mat, aux);

	TRANS.insertExtMatrix(mat);

	TRANS.insertInterN(names);

	intTransitions(TRANS, staC1, staC2, limits);

	TRANS.showSurvs();
	TRANS.showTransitions();

	return TRANS;

	

}