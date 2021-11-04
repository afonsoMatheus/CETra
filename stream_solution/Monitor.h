#ifndef MONITOR_H
#define MONITOR_H

#include <unordered_map>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <set>
#include <initializer_list>
#include <string>
#include <deque>
#include <exception>
#include "Transitions.h"

using namespace std;

struct int_statistic{

	vector<float> values;
	float lim;

	int_statistic(vector<float> v, int l = 5){

		values = v;
		lim = l;
	}

};

template <typename S = int, typename C = int>
class Monitor{

	using clustering = unordered_map<C, vector<S> >;
	using overlaping = unordered_map<C, vector<float> >;
	using search_table = unordered_map<S, tuple<C, float>>;
	using statistics = unordered_map<C, vector<float>>;

	private:

		int winSize;
		vector<float> limits;

		clustering clusR;
		unordered_map<C,float> cluW;
		statistics staR;
		
		vector<C> labels;
		statistics staE;
		
		overlaping matrix;

		Transitions<C> TRANS;
		deque<statistics> qTrans;
		deque<statistics> qSurvs;

		void storeClustering(const vector<S>&, const vector<C>&, const vector<float>&, const vector<C>&);

		void checkEvolution(const vector<S>&, const vector<C>&, const vector<float>&, const vector<C>&);

		template <class T>
		void buildStatistics(const vector<S> & ,const vector<C> &, const vector<C> &, initializer_list<T>);

		void clusterWeights(const vector<S>&, const vector<float>&);
		unordered_map<S, float> senWei(const vector<S>&, const vector<float>&);

		void intTransitions();

		////////////////////////////////////////////////////////////////////

		void storeLabels(const vector<C>);

		void clusterOverlap(const vector<S>&, const vector<C>&, const vector<float>&, const vector<C>&);
		unordered_map<C,int> useLabels();

		search_table makeHash(const vector<S>&, const vector<C>&, const vector<float>&, const vector<C>&);

		//////////////////////////////////////////////////////////////////

		void extTransitions();

		float sumSplits(const vector<float>&, const vector<C>&);

		unordered_map<int,C> hashLabels(const vector<C>&);

		void setSizes(const vector<S> &, const vector<C> &, const vector<C> &);

		template <class T>
		void storeRefStatistics(const vector<C> &, initializer_list<T>);

		template <class T>
		void storeEvoStatistics(const vector<S> &, const vector<C> &, const vector<C> &, initializer_list<T> );

		void freeRef();

		void freeEvo();

		void seeQueue();

		void seeExQueue();


	public:

		Monitor(const int N = 5, const vector<float> lim = {}){

			winSize = N;
			limits = lim;
			limits.insert(limits.begin(), 0.5);

		}

		void execute(const vector<S> &, const vector<C> &, const vector<float> &, const vector<C> &);

		template <class T>
		void execute(const vector<S> &, const vector<C> &, const vector<float> &, const vector<C> &, initializer_list<T> );

		//////////////////////////////////////////////////////////////////

		void setSizeLimit(const float&);

		void showStatistics();

		void showExtransitions();

		void showSurvs();

		void showSplits();

		void showUnions();

		void showDeaths();

		void showBirths();
	
};

template <typename S, typename C>
template <class T>
void Monitor<S,C>::execute(const vector<S> &sen, const vector<C> &clu, const vector<float> &wei, const vector<C> &lab, initializer_list<T> list){

	try{

		if(sen.size() != clu.size() or sen.size() != wei.size()){
			throw invalid_argument("The sensors/clusters/weights arrays are not the same size!");
		}

		if (list.size() + 1 != limits.size()){
			throw invalid_argument("The limits/statistics arrays are not the same size!");
		}

		if(clusR.empty()){

			storeClustering(sen, clu, wei, lab);
			
			storeRefStatistics(lab, list);

		}else{
			
			storeEvoStatistics(sen, clu, lab, list);

			checkEvolution(sen, clu, wei, lab);

			if(TRANS.checkExt()){

				freeRef();

				storeClustering(sen, clu, wei, lab);

				staR = staE;

			}

			freeEvo();

		}

	}catch(invalid_argument& e){

		cerr << e.what() << endl;

	}

}

template <typename S, typename C>
template <class T>
void Monitor<S,C>::storeRefStatistics(const vector<C> &lab, initializer_list<T> list){

	//inserindo tamanhos como estatistica
	for (auto &x: clusR){
		staR[x.first].insert(staR[x.first].end(), x.second.size()); 
	}

	for(auto s: list){
		int i = 0;

		if(s.values.size() != lab.size()){
			throw invalid_argument("The labels/statistics arrays are not the same size!");
		}

		for(auto x: s.values){
			staR[lab[i]].insert(staR[lab[i]].end(), x);
			i++;
		}		 
	}		

}

template <typename S, typename C>
template <class T>
void Monitor<S,C>::storeEvoStatistics(const vector<S> &sen, const vector<C> &clu, const vector<C> &lab, initializer_list<T> list){

	setSizes(sen, clu, lab);

	for(auto s: list){
		int i = 0;

		if(s.values.size() != lab.size()){
			throw invalid_argument("The labels/statistics arrays are not the same size!");
		}

		for(auto x: s.values){
			staE[lab[i]].insert(staE[lab[i]].end(), x);
			i++;
		}		 
	}		

}




#endif