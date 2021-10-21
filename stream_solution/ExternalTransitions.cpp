#include <iostream>
#include "HashSol.h"
#include "Transitions.cpp"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <set>
using namespace std;

using statistics = unordered_map<int ,tuple<float, float, float>>;

float sumSplits(const vector<float> &overlaps, const vector<int> &split_cand){

	float sum = 0;

	for(const auto &x: split_cand){
		sum+=overlaps[x];
	}

	return sum;

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

void extTransitions(overlaping &matrix, const vector<int> &labels){

	unordered_map<int,int> lmap = hashLabels(labels);

	unordered_map<int, vector<int>> scands;
	set<int> tracks;

	Transitions trans;

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

			}else if(mcell >= 0.25){

				split_cand.insert(split_cand.end(), Y);

			}

		}

		if(surv_cand == -2 && split_cand.empty() == true){

			trans.deaths.insert(trans.deaths.end(), X.first);

		}else if(split_cand.empty() == false){

			if(sumSplits(X.second, split_cand) >= 0.5){

				for(const auto x: split_cand) {
					trans.splits[X.first].insert(trans.splits[X.first].end(), lmap[x]);

					tracks.insert(lmap[x]);
				};
				cout << endl <<  endl;

			
			}else if(surv_cand != -2){

				scands[surv_cand].insert(scands[surv_cand].end(), X.first);
								
			}else{

				trans.deaths.insert(trans.deaths.end(), X.first);

			}

		}else{

			scands[surv_cand].insert(scands[surv_cand].end(), X.first);

		}

	}

	for(const auto &x: scands){

		if(scands[x.first].size() == 1){

			trans.survs.insert(trans.survs.begin(), make_tuple(scands[x.first][0], x.first));

			tracks.insert(x.first);
					
		}else{

			trans.absors[x.first] = x.second;

			tracks.insert(x.first);

		} 

	}

	set_difference(labels.begin(), labels.end(), tracks.begin(), tracks.end(),
        inserter(trans.births, trans.births.begin()));


	cout << "---EXT TRANSITIONS---" << endl << endl; 

	trans.showSurvs();

	trans.showSplits();

	trans.showAbsors();

	trans.showDeaths();

	trans.showBirths();

	
}

statistics buildStatistics(const clustering &clusR){

	statistics cluS;

	for(const auto &x: clusR){
		cluS[x.first] = make_tuple(x.second.size(), 0, 0);

		cout << x.first << ": " << get<0>(cluS[x.first]) << endl; 
	}

	return cluS;

}


int main(int argc, char const *argv[]){
	
	const vector<int> sensors1 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters1 = {1,1,2,3,3,3,3,3};
	const vector<float> weights1 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};

	clustering clusR = storeClusters(clusters1, sensors1);

	//statistics cluS = buildStatistics(clusR);

	unordered_map<int,float> cluW = clusterWeights<int, float>(sensors1, weights1, clusR);
	
	////////////////////////////////////////////////////////////////

	cout << endl << "///// NEXT WINDOW /////" << endl << endl;;

	const vector<int> sensors2 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters2 = {3,3,3,3,4,5,6,7};
	const vector<float> weights2 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
	vector<int> labels = {5,3,4,6,7}; //precisa ser ordenado
	
	unordered_map<int, tuple<int, float>> clusE = makeHash<int, int, float>(sensors2, clusters2, weights2);

	overlaping matrix = clusterOverlap(clusR, clusE, cluW, labels);

	////////////////////////////////////////////////////////////////

	extTransitions(matrix, labels);

	return 0;
}