#ifndef MONITOR_H
#define MONITOR_H

#include <unordered_map>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <set>
#include <initializer_list>

#include "Transitions.h"

using namespace std;

template <typename S = int, typename C = int, typename W = float>
class Monitor{

	using clustering = unordered_map<C, vector<S> >;
	using overlaping = unordered_map<C, vector<W> >;
	using statistics = unordered_map<C ,vector<float> >;

	private:

		clustering clusR;
		unordered_map<S,W> cluW;
		statistics cluS;
		
		vector<C> labels;
		unordered_map<S, tuple<C, W>> clusE;
		
		overlaping matrix;

		Transitions TRANS;

		void storeClustering(const vector<S>&, const vector<C>&, const vector<W>&, const vector<C>&);

		void checkEvolution(const vector<S>&, const vector<C>&, const vector<W>&, const vector<C>&);

		void buildStatistics();

		void clusterWeights(const vector<C>&, const vector<W>&);

		////////////////////////////////////////////////////////////////////

		void storeLabels(const vector<C>);

		void clusterOverlap();
		unordered_map<C,int> useLabels();

		unordered_map<C, W> makeHash(const vector<C>&, const vector<W>&);

		void makeHash(const vector<S>&, const vector<C>&, const vector<W>&, const vector<C>&);

		//////////////////////////////////////////////////////////////////

		Transitions extTransitions();

		W sumSplits(const vector<W>&, const vector<C>&);

		unordered_map<int,C> hashLabels(const vector<C>&);

	public:

		void execute(const vector<S> &, const vector<C> &, const vector<W> &, const vector<C> &);

		template <class T>
		void execute(const vector<S> &, const vector<C> &, const vector<W> &, const vector<C> &, initializer_list<T>);

		//////////////////////////////////////////////////////////////////

		void showSurvs();

		void showSplits();

		void showUnions();

		void showDeaths();

		void showBirths();
	
};


#endif