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

template <typename S = int, typename C = int, typename W = float>
class Monitor{

	struct ext_statistic{	

		vector<W> values;
		C c2;

		ext_statistic(){};

		ext_statistic(vector<W> v, C y){

			values = v;
			c2 = y;
		}

	};

	using clustering = unordered_map<C, vector<S> >;
	using overlaping = unordered_map<C, vector<W> >;
	using search_table = unordered_map<S, tuple<C, W>>;
	using statistics = unordered_map<C, vector<float>>;

	private:

		int winSize;

		vector<string> sta;

		clustering clusR;
		unordered_map<C,W> cluW;
		statistics staR;

		vector<C> labels;
		statistics staE;
		
		overlaping matrix;

		Transitions<C> TRANS;

		deque< unordered_map<C, ext_statistic >> qTrans;
		deque<statistics> qSurvs;

		void storeClustering(const vector<S>&, const vector<C>&, const vector<W>&, const vector<C>&);

		void checkEvolution(const vector<S>&, const vector<C>&, const vector<W>&, const vector<C>&);

		template <class T>
		void buildStatistics(const vector<S> & ,const vector<C> &, const vector<C> &, initializer_list<T>);

		void clusterWeights(const vector<S>&, const vector<W>&);
		unordered_map<S, W> senWei(const vector<S>&, const vector<W>&);

		void intTransitions();

		////////////////////////////////////////////////////////////////////

		void storeLabels(const vector<C>);

		void clusterOverlap(const vector<S>&, const vector<C>&, const vector<W>&, const vector<C>&);
		unordered_map<C,int> useLabels();

		search_table makeHash(const vector<S>&, const vector<C>&, const vector<W>&, const vector<C>&);

		//////////////////////////////////////////////////////////////////

		void extTransitions();

		W sumSplits(const vector<W>&, const vector<C>&);

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

		Monitor(const int N = 5, const vector<string> names = {}){

			winSize = N;
			sta = names;

		}

		void execute(const vector<S> &, const vector<C> &, const vector<W> &, const vector<C> &);

		template <class T>
		void execute(const vector<S> &, const vector<C> &, const vector<W> &, const vector<C> &, initializer_list<T> );

		//////////////////////////////////////////////////////////////////

		void showStatistics();

		void showExtransitions();

		void showSurvs();

		void showSplits();

		void showUnions();

		void showDeaths();

		void showBirths();
	
};

template <typename S, typename C, typename W>
template <class T>
void Monitor<S,C,W>::execute(const vector<S> &sen, const vector<C> &clu, const vector<W> &wei, const vector<C> &lab, initializer_list<T> list){

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

}

template <typename S, typename C, typename W>
template <class T>
void Monitor<S,C,W>::storeRefStatistics(const vector<C> &lab, initializer_list<T> list){

	//inserindo tamanhos como estatistica
	for (auto &x: clusR){
		staR[x.first].insert(staR[x.first].end(), x.second.size()); 
	}

	for(auto s: list){
		int i = 0;
		for(auto x: s.values){
			staR[lab[i]].insert(staR[lab[i]].end(), x);
			i++;
		}		 
	}		

}

template <typename S, typename C, typename W>
template <class T>
void Monitor<S,C,W>::storeEvoStatistics(const vector<S> &sen, const vector<C> &clu, const vector<C> &lab, initializer_list<T> list){

	setSizes(sen, clu, lab);

	for(auto s: list){
		int i = 0;
		for(auto x: s.values){
			staE[lab[i]].insert(staE[lab[i]].end(), x);
			i++;
		}		 
	}		

}



#endif