#include <iostream>
#include <vector>
#include <unordered_map>
#include "/home/afonso/Documentos/Dissertação/Masters Degree/tests/ReadFile.h"

using namespace std;

unordered_map<int, vector<float>> overlapMatrix(const Clustering &C1, const Clustering &C2, const vector<int> &aux){

	unordered_map<int, vector<float>> om;

	unordered_map<int,float> wei;

	unordered_map<int,int> lmap;

	int a = 0;
	for(const auto &x: aux){
		lmap[x] = a;
		a++;
	}

	for(auto i = 0; i < C1.sensors.size(); i++){

		if(om.find(C1.clusters[i]) == om.end())
			om[C1.clusters[i]] = vector<float>(aux.size());
		wei[C1.clusters[i]] = wei[C1.clusters[i]] + C2.weights[i];

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

			if (mcell > 0.5){
				
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

void Monic(const Clustering &C1, const Clustering &C2){

	vector<int> aux = C2.clusters; 
	sort( aux.begin(), aux.end() );
	aux.erase( unique( aux.begin(), aux.end() ), aux.end());

	unordered_map<int, vector<float>> mat = overlapMatrix(C1, C2, aux);

	Transitions<int> TRANS = extTransitions<int>(mat, aux);

	//TRANS.showSurvs();
	//TRANS.showTransitions();

}