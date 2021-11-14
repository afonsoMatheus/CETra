#ifndef MONITOR_H
#define MONITOR_H

#include <unordered_map>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_set>
#include <deque>
#include <exception>
#include <cmath>
#include <string>
#include "Transitions.h"

using namespace std;

template <typename S = int, typename C = int>
class Monitor{

	using clustering = unordered_map<C, vector<S> >;
	using overlaping = unordered_map<C, vector<float> >;
	using search_table = unordered_map<S, tuple<C, float>>;
	using statistics = unordered_map<C, vector<float>>;

	private:

		int winSize;
		vector<float> limits;
		vector<string> names;
		float surv_limit;
		float split_limit;

		clustering clusR;
		unordered_map<C,float> refW;
		statistics staR;

		//clustering clusE;
		//unordered_map<C,float> evoW;
		statistics staE;

		set<C> evoLabels;
		search_table evoTable;
		overlaping matrix;

		unordered_map<C, vector<S>> failS;
		unordered_set<S> ns;
		unordered_map<C, vector<S>> newS;
		
		Transitions<C> TRANS;
		deque<statistics> staQueue;

		///////////////////// REF. CLUSTERING /////////////////////

		void storeClustering(const vector<S>&, const vector<C>&, const vector<float>&);

		template <class T>
		void storeRefStatistics(initializer_list<T>);

		void freeRef();

		///////////////////// EVO. CLUSTERING /////////////////////

		void getEvoInformation(const vector<S>&, const vector<C>&, const vector<float>&);

		template <class T>
		void storeEvoStatistics(initializer_list<T>);

		void checkEvolution();

		void clusterOverlap();
		
		unordered_map<C,int> useLabels();

		void freeEvo();

		//////////////////// EXT. TRANSITIONS ////////////////////

		void extTransitions();

		float sumSplits(const vector<float>&, const vector<C>&);

		unordered_map<int,C> hashLabels();

		//////////////////// INT. TRANSITIONS ////////////////////
		
		void intTransitions();

		void checkIntTrans();

		///////////////////// VISUALIZATION ////////////////////

		void showRefClustering();

		void showStatistics();

		void showIntersection();

		void showOverlaping();

		void showTrackStatistics();

		void showTransitions();

		void showSurvs();

		void showSplits();

		void showUnions();

		void showDeaths();

		void showBirths();

		void showIntQueue();

		void showInterC(vector<string>);

	public:

		Monitor(const int N = 5, const vector<float> lim = {}){

			winSize = N;

			limits = lim;
			limits.insert(limits.begin(), 0.5);

			names.insert(names.begin(), "Sizes");

			surv_limit = 0.5;
			split_limit = surv_limit/2;

		}

		void execute(const vector<S>&, const vector<C> &, const vector<float> &);

		template <class T>
		void execute(const vector<S>&, const vector<C> &, const vector<float> &, initializer_list<T>);

		////////////////////// USER CONFIGS //////////////////////

		void configSizeLimit(const float&);

		void configStaNames(const vector<string>&);

		void configSurvLimit(const float&);

		void configSplitLimit(const float&);
	
};

template <typename S, typename C>
void Monitor<S, C>::execute(const vector<S> &sen, const vector<C> &clu, const vector<float> &wei){
	
	try{

		if(sen.size() != clu.size() or sen.size() != wei.size()){
			throw invalid_argument("The sensors/clusters/weights arrays are not the same size!");
		}

		if(clusR.empty()){

			storeClustering(sen, clu, wei);

			showRefClustering();

			for (const auto &x: clusR) staR[x.first].insert(staR[x.first].end(), x.second.size()); 

		}else{

			cout << endl << "///// NEXT WINDOW /////" << endl << endl;

			getEvoInformation(sen, clu, wei);

			checkEvolution();

			if(!TRANS.getSurvs().empty()) showSurvs();

			if(TRANS.checkExtChange() || TRANS.checkIntChange()){

				showTransitions();

				freeRef();

				storeClustering(sen, clu, wei);

				/*clusR = clusE;
				refW = evoW;*/
				staR = staE;

				showRefClustering();

			}

			freeEvo();
		}

	}catch(invalid_argument& e){

		cerr << e.what() << endl;

	}

}


template <typename S, typename C>
template <class T>
void Monitor<S,C>::execute(const vector<S> &sen, const vector<C> &clu, const vector<float> &wei, initializer_list<T> list){

	try{

		if(sen.size() != clu.size() or sen.size() != wei.size()){
			throw invalid_argument("The sensors/clusters/weights arrays are not the same size!");
		}

		if (list.size() + 1 < limits.size()){
			throw invalid_argument("The limits array size is greater than the input statistics list!");
		}
		
		if(names.size() > list.size() + 1){
			throw invalid_argument("The amount of statistics names surpasses the statistics list!");
		}

		if(limits.size() < list.size() + 1){
			for(auto i = limits.size(); i < list.size() + 1; i++){
				limits.insert(limits.end(), 0.5);
			}
		}

		if(names.size() < list.size() + 1){
			int j = 1;
			for(auto i = names.size(); i < list.size() + 1; i++){
				names.insert(names.end(), "Sta_" + to_string(j));
				j++;
			}
		}

		if(clusR.empty()){

			storeClustering(sen, clu, wei);
			
			storeRefStatistics(list);

			showRefClustering();

		}else{

			cout << endl << "/////////////// NEXT WINDOW ///////////////" << endl << endl;

			getEvoInformation(sen, clu, wei);
			
			storeEvoStatistics(list);

			checkEvolution();

			showTrackStatistics();

			if(!TRANS.getSurvs().empty()) showSurvs();

			if(TRANS.checkExtChange() || TRANS.checkIntChange()){

				cout << "=== TRANS DETECTED ===" << endl << endl;

				showTransitions();

				cout << "### UPD REF CLUSTERING ###" << endl << endl;

				freeRef();
				storeClustering(sen, clu, wei);
				//clusR = clusE;
				//refW = evoW;
				staR = staE;

				showRefClustering();

			}

			freeEvo();
		}

	}catch(invalid_argument& e){

		cerr << e.what() << endl;

		throw -1;

	}catch(length_error& e){

		cerr << e.what() << endl;

	}

}

////////////////////// USER CONFIGS //////////////////////

template <typename S, typename C>
void Monitor<S,C>::configSizeLimit(const float& sl){
	limits[0] = sl;}

template <typename S, typename C>
void Monitor<S,C>::configStaNames(const vector<string>& nam){
	for(const auto &x: nam) names.insert(names.end(), x);}

template <typename S, typename C>
void Monitor<S,C>::configSurvLimit(const float& ls){
	surv_limit = ls;}

template <typename S, typename C>
void Monitor<S,C>::configSplitLimit(const float& ls){
	split_limit = ls;}


///////////////////// REF. CLUSTERING /////////////////////

template <typename S, typename C>
void Monitor<S, C>::storeClustering(const vector<S> &sen, const vector<C> &clu, const vector<float> &wei){

	for (int i = 0; i < sen.size(); ++i){
		clusR[clu[i]].insert(clusR[clu[i]].end(), sen[i]);
		refW[clu[i]] += wei[i];
	}
}

template <typename S, typename C>
template <class T>
void Monitor<S,C>::storeRefStatistics(initializer_list<T> list){

	set<C> ref_labels;

	//inserindo tamanhos como estatistica
	for (const auto &x: clusR){
		if(ref_labels.find(x.first) == ref_labels.end()) ref_labels.insert(x.first);
		
		staR[x.first].insert(staR[x.first].end(), x.second.size()); 
	}

	for(auto &s: list){
		auto itr = ref_labels.begin();

		if(s.size() != ref_labels.size()){
			throw invalid_argument("The labels/statistics arrays are not the same size!");
		}

		for(const auto &x: s){
			staR[*itr].insert(staR[*itr].end(), x);
			itr++;
		}		 
	}		

}

template <typename S, typename C>
void Monitor<S,C>::freeRef(){

	clusR.clear();
	refW.clear();
	staR.clear();
	staQueue = {};

}

///////////////////// EVO. CLUSTERING /////////////////////

template <typename S, typename C>
void Monitor<S,C>::getEvoInformation(const vector<S> &sen, const vector<C> &clu, const vector<float> &wei){

	for (int i = 0; i < sen.size(); ++i){

		//clusE[clu[i]].insert(clusE[clu[i]].end(), sen[i]);
		//evoW[clu[i]] += wei[i];

		if(evoLabels.find(clu[i]) == evoLabels.end()) 
			evoLabels.insert(clu[i]);

		evoTable[sen[i]] = make_tuple(clu[i],wei[i]);

		if(staE.find(clu[i]) == staE.end())
			staE[clu[i]].insert(staE[clu[i]].end(), 0);

		//updating size
		staE[clu[i]][0] += 1;

		ns.insert(sen[i]);
	}

}

template <typename S, typename C>
template <class T>
void Monitor<S,C>::storeEvoStatistics(initializer_list<T> list){

	for(const auto &s: list){
		auto itr = evoLabels.begin();

		if(s.size() != evoLabels.size()){
			throw invalid_argument("The labels/statistics arrays are not the same size!");
		}

		for(const auto &x: s){

			staE[*itr].insert(staE[*itr].end(), x);
			itr++;
		}		 
	}		

}

template <typename S, typename C>
void Monitor<S, C>::checkEvolution(){

	clusterOverlap();

	extTransitions();

	intTransitions();

}

template <typename S, typename C>
void Monitor<S, C>::clusterOverlap(){
	
	unordered_map<C,int> lmap = useLabels();

	for (auto &c : clusR){
		matrix[c.first] = vector<float>(evoLabels.size()); //saber quantidade de grupos do agrupamento 2
		for (auto &elem : c.second) {
			if(evoTable.find(elem) != evoTable.end()){
				matrix[c.first][lmap[get<0>(evoTable[elem])]]+= get<1>(evoTable[elem]);
				ns.erase(elem);
			}else{
				failS[c.first].insert(failS[c.first].end(),elem);
			};
		}
	}

	for(const auto &x: ns){
		newS[get<0>(evoTable[x])].insert(newS[get<0>(evoTable[x])].end(), x);
	}

	showIntersection();


	for (const auto &i : matrix){
		for (auto x = 0; x < i.second.size(); x++){
			matrix[i.first][x] =  matrix[i.first][x]/refW[i.first];
		}
	}

	showOverlaping();

}

template <typename S, typename C>
unordered_map<C,int> Monitor<S, C>::useLabels(){

	unordered_map<C, int> lmap;

	int i = 0;
	for(const auto &x: evoLabels){
		lmap[x] = i;
		i++;
	}

	return lmap;

}

template <typename S, typename C>
void Monitor<S,C>::freeEvo(){

	//clusE.clear();
	//evoW.clear();
	staE.clear();
	
	evoLabels.clear();
	evoTable.clear();
	matrix.clear();

	failS.clear();
	ns.clear();
	newS.clear();

	TRANS.clear();	
	
}

//////////////////// EXT. TRANSITIONS ////////////////////

template <typename S, typename C>
void Monitor<S, C>::extTransitions(){

	unordered_map<int,C> lmap = hashLabels();

	unordered_map<C, vector<C>> scands;
	set<C> tracks;

	for(const auto &X : matrix){

		vector<C> split_cand;

		C split_union = -2;
		C surv_cand = -2;

		for (int Y = 0; Y < X.second.size(); Y++){

			float mcell = X.second[Y];

			if (mcell > surv_limit){
				
				if(surv_cand = -2){
					surv_cand = lmap[Y];
				
				}else if(mcell > X.second[surv_cand]){

					surv_cand = lmap[Y];
				
				}else if(mcell == X.second[surv_cand]){

					if(clusR[surv_cand].size() < clusR[lmap[Y]].size()){
						surv_cand = lmap[Y];
					}

				}

			}else if(mcell >= split_limit) {
				
				split_cand.insert(split_cand.end(), Y);
			}

		}

		if(surv_cand == -2 && split_cand.empty() == true){

			TRANS.insertDeath(X.first);

		}else if(split_cand.empty() == false){

			if(sumSplits(X.second, split_cand) >= surv_limit){

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

	set_difference(evoLabels.begin(), evoLabels.end(), tracks.begin(), tracks.end(),
        inserter(TRANS.allocBirths(), TRANS.allocBirths().begin()));
	
}

template <typename S, typename C>
unordered_map<int,C> Monitor<S, C>::hashLabels(){

	unordered_map<int, C> umap;

	int i = 0;
	for(const auto &x: evoLabels){
		umap[i] = x;
		i++;
	}
	return umap;

}

template <typename S, typename C>
float Monitor<S, C>::sumSplits(const vector<float> &overlaps, const vector<C> &split_cand){

	float sum = 0;

	for(const auto &x: split_cand){
		sum+=overlaps[x];
	}

	return sum;
}

//////////////////// INT. TRANSITIONS ////////////////////

template <typename S, typename C>
void Monitor<S, C>::intTransitions(){

	statistics inter;

	for(const auto &x: TRANS.getSurvs()){
		for(int i = 0; i < staR[get<0>(x)].size(); i++){
			inter[get<0>(x)].insert(inter[get<0>(x)].end(), staE[get<1>(x)][i]/staR[get<0>(x)][i]);
		}
	}

	if(staQueue.size() < winSize){
		staQueue.push_back(inter);
	}else{
		staQueue.pop_front();
		staQueue.push_back(inter);
	}

	if(staQueue.size() == winSize) checkIntTrans();
	
}

template <typename S, typename C>
void Monitor<S,C>::checkIntTrans(){

	unordered_map<C, vector<int>> counts;

	for(const auto &x: clusR){
		for(int i = 0; i < limits.size(); i++) counts[x.first].insert(counts[x.first].end(), 0);
	} 

	for(const auto &cluS: staQueue){
		for(const auto &clu: cluS){
			for(int i = 0; i < clu.second.size(); i++){
				if(fabs(clu.second[i]-1) > limits[i]){
					counts[clu.first][i]+=1;
				}
			}
		}	
	}

	for(const auto &x: counts){
		int i = 0;
		for(const auto &y: x.second){
			if(y == staQueue.size()) {
				TRANS.insertInterC(x.first, i);
			}
			i++;
		}
	}

}

///////////////////// VISUALIZATION ////////////////////

template <typename S, typename C>
void Monitor<S, C>::showRefClustering(){

	cout << "------Clustering------" << endl;
	for (const auto &i : clusR){
		cout << i.first << ": "; 
		for (const auto &x : i.second) cout << x << " ";
		cout << endl;
	}

	cout << "----Cluster Weights----" << endl;
	for(const auto &x : refW) cout << x.first << ": " << x.second << endl;

}


template <typename S, typename C>
void Monitor<S, C>::showStatistics(){
	
	cout << "------Statistics-------" << endl;

	for(const auto &x: staR){
		cout << x.first << ": ";
		for(const auto &y: x.second){
			cout << "( " << y << " )";
		}
		cout << endl;
	}

	cout << endl;

	for(const auto &x: staE){
		cout << x.first << ": ";
		for(const auto &y: x.second){
			cout << "( " << y << " )";
		}
		cout << endl;
	}

	cout << endl;

}

template <typename S, typename C>
void Monitor<S,C>::showIntersection(){

	cout << "-----Intersection-----" << endl;
	for (const auto &i : matrix){
		cout << i.first << ": "; 
		for (const auto &x : i.second) cout << x << " ";
		cout << endl;
	}
	cout << endl;
}

template <typename S, typename C>
void Monitor<S,C>::showOverlaping(){

	cout << "-----Overlapping-----" << endl;
	for (const auto &i : matrix){
		cout << i.first << ": "; 
		for (const auto &x : i.second) cout << x << " ";
		cout << endl;
	}
	cout << endl;
}

template <typename S, typename C>
void Monitor<S,C>::showTrackStatistics(){

	if(!failS.empty()){

		cout << "Faulty sensors" << endl;
		for(const auto &x: failS){
			cout << x.first << ": ";
			for(const auto &y: x.second) cout << y << " ";
			cout << endl;
		}
		cout << endl;
	}
	
	if(!newS.empty()){
		cout << "New sensors" << endl;
		for(const auto &x: newS){
			cout << x.first << ": ";
			for(const auto &y: x.second) cout << y << " ";
			cout << endl;
		}
		cout << endl;
	}
}


template <typename S, typename C>
void Monitor<S, C>::showTransitions(){

	if(TRANS.checkExtChange()){

		cout << "+ EXT TRANSITIONS" << endl << endl; 

		if(!TRANS.getSplits().empty()) showSplits();

		if(!TRANS.getUnions().empty()) showUnions();

		if(!TRANS.getDeaths().empty()) showDeaths();

		if(!TRANS.getBirths().empty()) showBirths();

	}

	if(TRANS.checkIntChange()){

		cout << "+ INT TRANSITIONS" << endl << endl;

		if(!TRANS.getInterC().empty()) showInterC(names);

		cout << endl;

	}
	
}

template <typename S, typename C>
void Monitor<S, C>::showSurvs(){
	TRANS.showSurvs();
}

template <typename S, typename C>
void Monitor<S, C>::showSplits(){
	TRANS.showSplits();
}

template <typename S, typename C>
void Monitor<S, C>::showUnions(){
	TRANS.showUnions();
}

template <typename S, typename C>
void Monitor<S, C>::showDeaths(){
	TRANS.showDeaths();
}

template <typename S, typename C>
void Monitor<S, C>::showBirths(){
	TRANS.showBirths();
}


template <typename S, typename C>
void Monitor<S,C>::showIntQueue(){

	for(const auto &x: staQueue){
		cout << "----------" << endl;
		for(const auto &z: x){
			cout << z.first << ": ";
			for(const auto &y: z.second){
				cout << y << " ";
			}
			cout << endl;
		}
	}

}

template <typename S, typename C>
void Monitor<S, C>::showInterC(const vector<string> nam){
	TRANS.showInterC(nam);
}


#endif