#ifndef TRANSITIONS_H
#define TRANSITIONS_H

using namespace std;

#include <vector>
#include <tuple>
#include <unordered_map>
#include <iostream>

template <typename T>
class Transitions{

	private:
		
		vector<tuple<T,T>> survs;
		
		unordered_map<T, vector<T>> unions;
		unordered_map<T, vector<T>> splits;

		vector<T> deaths;
		vector<T> births;

	public:		

		Transitions();

		void clear();

		///////////////////////////////////////////

		void insertDeath(T D);

		vector<T> getDeaths();

		void insertSurv(tuple<T,T> S);

		vector<tuple<T,T>> getSurvs();

		void insertUnion(T C2, vector<T> C1);

		unordered_map<T, vector<T>> getUnions();

		void insertSplits(T C1, T C2);

		unordered_map<T, vector<T>> getSplits();

		vector<T>& allocBirths();

		vector<T> getBirths();

		bool checkExt();

		///////////////////////////////////////////

		void showDeaths();

		void showSplits();

		void showBirths();

		void showSurvs();

		void showUnions();

};


#endif