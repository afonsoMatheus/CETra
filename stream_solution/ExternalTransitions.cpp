#include <iostream>
#include "HashSol.h"
#include <vector>
#include <unordered_map>

using namespace std;

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

	vector<int> deaths;
	vector<int> tracked;
	unordered_map<int, vector<int>> absors;
	unordered_map<int,int> lmap = hashLabels(labels);


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

			deaths.insert(deaths.end(), X.first);

		}else if(split_cand.empty() == false){

			if(sumSplits(X.second, split_cand) >= 0.5){

				cout << "Separações" << endl << X.first << " -> ";
				for(const auto x: split_cand) {
					cout << lmap[x] << " (" << X.second[x] << ") ";
					tracked.insert(tracked.end(), lmap[x]);
				};
				cout << endl <<  endl;

			
			}else if(surv_cand != -2){

				absors[surv_cand].insert(absors[surv_cand].end(), X.first);
				tracked.insert(tracked.end(), lmap[surv_cand]);

			}else{

				deaths.insert(deaths.end(), X.first);

			}

		}else{

			absors[surv_cand].insert(absors[surv_cand].end(), X.first);

		}

	}

	for(auto &x: absors){

		if(absors[x.first].size() > 1){

			cout << "Uniões" << endl;
			for(const auto y: x.second) cout << y << " ";
			cout << "-> " << x.first << endl << endl;

			tracked.insert(tracked.end(), x.first);

		}else{

			cout << "Sobreviventes" << endl; 
			cout << absors[x.first][0] << " -> " << x.first  << endl;
			cout << endl;

			tracked.insert(tracked.end(), x.first);
		}

	}

	cout << "Mortes" << endl;
	for(const auto &x : deaths){
		cout << x << " -> DEATH " << endl;
	}
	cout << endl;

	cout << "Nascimentos" << endl;
	for(auto x = 0;  x < labels.size(); x++){
		
		for(auto y = 0; y < tracked.size(); y++){
			
			if(labels[x] == tracked[y])break;

			if(y == tracked.size()-1) cout << "BIRTHS -> " << labels[x] << endl;
		}	
		
	}


}



int main(int argc, char const *argv[]){
	
	const vector<int> sensors1 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters1 = {0,0,0,0,1,1,1,0};
	const vector<float> weights1 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};

	clustering clusR = storeClusters(clusters1, sensors1);

	unordered_map<int,float> cluW = clusterWeights<int, float>(sensors1, weights1, clusR);
	
	////////////////////////////////////////////////////////////////

	cout << endl << "///// NEXT WINDOW /////" << endl << endl;;

	const vector<int> sensors2 = {1,2,3,4,5,6,7,8};
	const vector<int> clusters2 = {3,3,4,4,3,3,4,4};
	const vector<float> weights2 = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
	vector<int> labels = {4,3}; //precisa ser ordenado
	
	unordered_map<int, tuple<int, float>> clusE = makeHash<int, int, float>(sensors2, clusters2, weights2);

	overlaping matrix = clusterOverlap(clusR, clusE, cluW, labels);

	////////////////////////////////////////////////////////////////

	extTransitions(matrix, labels);

	return 0;
}