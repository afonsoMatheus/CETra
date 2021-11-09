#ifndef MONITOR_H
#define MONITOR_H

#include <unordered_map>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <set>
#include <array>
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
		float new_limit;
		float fail_limit;

		int sizeR;
		clustering clusR;
		unordered_map<C,float> cluW;
		statistics staR;
		
		vector<C> labels;
		statistics staE;
		
		overlaping matrix;

		Transitions<C> TRANS;
		deque<statistics> qTrans;
		deque<statistics> qSurvs;

		void storeClustering(const vector<S>&, const vector<C>&, const vector<float>&);

		void checkEvolution(const vector<S>&, const vector<C>&, const vector<float>&);

		template <class T>
		void buildStatistics(const vector<S> & ,const vector<C> &, const vector<C> &, initializer_list<T>);

		void clusterWeights(const vector<S>&, const vector<float>&);
		unordered_map<S, float> senWei(const vector<S>&, const vector<float>&);

		void intTransitions();

		////////////////////////////////////////////////////////////////////

		void storeEvoLabels(const vector<C>&);

		void clusterOverlap(const vector<S>&, const vector<C>&, const vector<float>&);
		unordered_map<C,int> useLabels();

		search_table makeSearchTable(const vector<S>&, const vector<C>&, const vector<float>&);

		//////////////////////////////////////////////////////////////////

		void extTransitions();

		float sumSplits(const vector<float>&, const vector<C>&);

		unordered_map<int,C> hashLabels(const vector<C>&);

		void setSizes(const vector<S> &, const vector<C> &);

		template <class T>
		void storeRefStatistics(const vector<C> &, initializer_list<T>);

		template <class T>
		void storeEvoStatistics(const vector<S> &, const vector<C> &, initializer_list<T> );

		void freeRef();

		void freeEvo();

		void seeIntQueue();

		void seeExQueue();

		void checkIntTrans();

	public:

		Monitor(const int N = 5, const vector<float> lim = {}){

			winSize = N;

			limits = lim;
			limits.insert(limits.begin(), 0.5);

			names.insert(names.begin(), "Sizes");

			new_limit = 0.5;
			fail_limit = 0.5;

		}

		void execute(const vector<S>&, const vector<C> &, const vector<float> &, const vector<C> &);

		template <class T>
		void execute(const vector<S>&, const vector<C> &, const vector<float> &, const vector<C> &, initializer_list<T> );

		//////////////////////////////////////////////////////////////////

		void configSizeLimit(const float&);

		void configStaNames(const vector<string>&);

		void configNewLimit(const float&);

		void configFailLimit(const float&);

		void showStatistics();

		void showTransitions();

		void showSurvs();

		void showSplits();

		void showUnions();

		void showDeaths();

		void showBirths();

		void showInterC(const vector<string>);
	
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

	if(names.size() > list.size() + 1){
		throw invalid_argument("The amount of statistics names surpasses the statistics list!");
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
		
		storeRefStatistics(lab, list);

	}else{

		cout << endl << "///// NEXT WINDOW /////" << endl << endl;

		storeEvoLabels(lab);
		
		storeEvoStatistics(sen, clu, list);

		if(TRANS.checkNewRatio(new_limit)){

			cout << "====TOO MANY NEW SAMPLES====" << endl;
			cout << "===UPDATING REF CLUSTERING===" << endl << endl;

			freeRef();
			storeClustering(sen, clu, wei);
			staR = staE;
		
		}else{

			checkEvolution(sen, clu, wei);

			if(!TRANS.getSurvs().empty()) showSurvs();

			if(TRANS.checkExtChange() || TRANS.checkIntChange()){

				cout << "====TRANS DETECTED====" << endl << endl;

				showTransitions();

				freeRef();

				cout << "### NEW REF CLUSTERING ###" << endl << endl;

				storeClustering(sen, clu, wei);

				staR = staE;

			}

		}

		freeEvo();
	}

	}catch(invalid_argument& e){

		cerr << e.what() << endl;

		throw -1;

	}catch(domain_error& e){

		cerr << e.what() << endl;

	}

}

template <typename S, typename C>
template <class T>
void Monitor<S,C>::storeRefStatistics(const vector<C> &lab, initializer_list<T> list){

	//inserindo tamanhos como estatistica
	for (const auto &x: clusR){
		staR[x.first].insert(staR[x.first].end(), x.second.size()); 
	}

	for(auto &s: list){
		int i = 0;

		if(s.size() != lab.size()){
			throw invalid_argument("The labels/statistics arrays are not the same size!");
		}

		for(const auto &x: s){
			staR[lab[i]].insert(staR[lab[i]].end(), x);
			i++;
		}		 
	}		

}

template <typename S, typename C>
template <class T>
void Monitor<S,C>::storeEvoStatistics(const vector<S> &sen, const vector<C> &clu, initializer_list<T> list){

	setSizes(sen, clu);

	for(const auto &s: list){
		int i = 0;

		if(s.size() != labels.size()){
			throw invalid_argument("The labels/statistics arrays are not the same size!");
		}

		for(const auto &x: s){
			staE[labels[i]].insert(staE[labels[i]].end(), x);
			i++;
		}		 
	}		

}

#endif