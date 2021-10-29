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
#include <queue>
#include "Transitions.h"

using namespace std;

template <typename S = int, typename C = int, typename W = float>
class Monitor{

	using clustering = unordered_map<C, vector<S> >;
	using overlaping = unordered_map<C, vector<W> >;
	using statistics = unordered_map<C, vector<float>>;

	private:

		int winSize;

		vector<string> sta;

		clustering clusR;
		unordered_map<C,W> cluW;
		statistics staR;

		vector<C> labels;
		unordered_map<S, tuple<C, W>> clusE;
		statistics staE;
		
		overlaping matrix;

		Transitions<C> TRANS;

		statistics temp;
		queue<statistics> qSurvs;

		void storeClustering(const vector<S>&, const vector<C>&, const vector<W>&, const vector<C>&);

		void checkEvolution(const vector<S>&, const vector<C>&, const vector<W>&, const vector<C>&);

		template <class T>
		void buildStatistics(const vector<C>& ,initializer_list<T>);

		void clusterWeights(const vector<S>&, const vector<W>&);
		unordered_map<S, W> senWei(const vector<S>&, const vector<W>&);

		void intTransitions();

		void compare(const float&, const float&);

		////////////////////////////////////////////////////////////////////

		void storeLabels(const vector<C>);

		void clusterOverlap();
		unordered_map<C,int> useLabels();


		void makeHash(const vector<S>&, const vector<C>&, const vector<W>&, const vector<C>&);

		//////////////////////////////////////////////////////////////////

		Transitions<C> extTransitions();

		W sumSplits(const vector<W>&, const vector<C>&);

		unordered_map<int,C> hashLabels(const vector<C>&);

	public:

		//Monitor();

		Monitor(const int N = 5, const vector<string> names = {}){

			winSize = N;
			sta = names;

		}

		void execute(const vector<S> &, const vector<C> &, const vector<W> &, const vector<C> &);

		template <class T>
		void execute(const vector<S> &, const vector<C> &, const vector<W> &, const vector<C> &, initializer_list<T>);

		//////////////////////////////////////////////////////////////////

		void showStatistics();

		void showExtransitions();

		void showSurvs();

		void showSplits();

		void showUnions();

		void showDeaths();

		void showBirths();
	
};


#endif