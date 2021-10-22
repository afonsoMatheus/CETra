#include "ExternalMon.h"

Transitions extTransitions(const overlaping &matrix, const vector<int> &labels){

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

unordered_map<int,int> hashLabels(const vector<int> &labels){

	unordered_map<int, int> umap;

	int i = 0;
	for(const auto &x: labels){
		umap[i] = x;
		i++;
	}

	return umap;

}

statistics buildStatistics(const clustering &clusR){

	statistics cluS;

	for(const auto &x: clusR){
		cluS[x.first] = make_tuple(x.second.size(), 0, 0);

		cout << x.first << ": " << get<0>(cluS[x.first]) << endl; 
	}

	return cluS;

}

float sumSplits(const vector<float> &overlaps, const vector<int> &split_cand){

	float sum = 0;

	for(const auto &x: split_cand){
		sum+=overlaps[x];
	}

	return sum;
}
