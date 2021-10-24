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

using clustering = unordered_map<int, vector<int>>;
using overlaping = unordered_map<int, vector<float>>;
using statistics = unordered_map<int ,tuple<float, float, float>>;


class Monitor{
	
	private:

		clustering clusR;
		unordered_map<int,float> cluW;

		vector<int> labels;
		unordered_map<int, tuple<int, float>> clusE;
		
		overlaping matrix;

		Transitions TRANS;

		unordered_map<int,int> useLabels();
		void clusterOverlap();

		template <typename T, typename U >
		unordered_map<T, U> makeHash(const vector<T>&, const vector<U>&);

		//////////////////////////////////////////////////////////////////

		Transitions extTransitions(const overlaping &, const vector<int> &);

		float sumSplits(const vector<float> &, const vector<int> &);

		unordered_map<int,int> hashLabels(const vector<int> &);

		statistics buildStatistics(const clustering &);

	public:

		void storeClusters(const vector<int>&, const vector<int>&);

		template <typename T, typename U >
		void clusterWeights(const vector<T>&, const vector<U>&);

		template <typename T, typename U , typename V>
		void makeHash(const vector<T>&, const vector<U>&, const vector<V>&);

		void storeLabels(vector<int>);

		void Run();

		void showSurvs();
		void showSplits();
		void showUnions();
		void showDeaths();
		void showBirths();

};


#endif