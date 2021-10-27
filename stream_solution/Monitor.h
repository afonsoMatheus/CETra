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

using cluster_data = vector<vector<float>>;


template <typename S = int, typename C = int, typename W = float>
class Monitor{

	using clustering = unordered_map<C, vector<S> >;
	using overlaping = unordered_map<C, vector<W> >;
	using statistics = unordered_map<C, cluster_data>;
	

	private:

		clustering clusR;
		unordered_map<S,W> cluW;
		statistics staR;

		vector<C> labels;
		unordered_map<S, tuple<C, W>> clusE;
		statistics staE;
		
		overlaping matrix;

		Transitions TRANS;

		void storeClustering(const vector<S>&, const vector<C>&, const vector<W>&, const vector<C>&);

		void checkEvolution(const vector<S>&, const vector<C>&, const vector<W>&, const vector<C>&);

		template <class T>
		void buildStatistics(const vector<C>& ,initializer_list<T>);

		void clusterWeights(const vector<C>&, const vector<W>&);
		
		void intTransitions();

		void compare(const vector<float>&, const vector<float>&);

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

		void showStatistics();

		void showSurvs();

		void showSplits();

		void showUnions();

		void showDeaths();

		void showBirths();
	
};


#endif