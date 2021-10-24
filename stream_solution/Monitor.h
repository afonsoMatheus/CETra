#ifndef MONITOR_H
#define MONITOR_H

#include <unordered_map>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <set>
#include "Transitions.h"

using namespace std;

template <typename S, typename C, typename W>
class Monitor{

	using clustering = unordered_map<C, vector<S> >;
	using overlaping = unordered_map<C, vector<W> >;
	using statistics = unordered_map<C ,tuple<float, float, float> >;

	private:

		clustering clusR;
		unordered_map<S,W> cluW;
		statistics cluS;
		
		vector<C> labels;
		unordered_map<S, tuple<C, W>> clusE;
		
		overlaping matrix;

		Transitions TRANS;

		void buildStatistics();

		void clusterWeights(const vector<C>&, const vector<W>&);

		////////////////////////////////////////////////////////////////////

		void storeLabels(const vector<C>);

		void clusterOverlap();
		unordered_map<C,int> useLabels();

		unordered_map<C, W> makeHash(const vector<C>&, const vector<W>&);

		void makeHash(const vector<S>&, const vector<C>&, const vector<W>&, const vector<C> &);

		//////////////////////////////////////////////////////////////////

		Transitions extTransitions();

		W sumSplits(const vector<W> &, const vector<C> &);

		unordered_map<int,C> hashLabels(const vector<C> &);


	public:

		void relClustering(const vector<S>&, const vector<C>&, const vector<W> &);

		void checkEvolution(const vector<S>&, const vector<C>&, const vector<W>&, const vector<C> &);

		//////////////////////////////////////////////////////////////////

		void showSurvs();

		void showSplits();

		void showUnions();

		void showDeaths();

		void showBirths();
		

	
};




#endif