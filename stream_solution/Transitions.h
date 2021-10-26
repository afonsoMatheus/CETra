#ifndef TRANSITIONS_H
#define TRANSITIONS_H

using namespace std;

#include <vector>
#include <tuple>
#include <unordered_map>
#include <iostream>

class Transitions{

	private:
		
		vector<tuple<int,int>> survs;
		
		unordered_map<int, vector<int>> unions;
		unordered_map<int, vector<int>> splits;

		vector<int> deaths;
		vector<int> births;

	public:		

		Transitions();

		///////////////////////////////////////////

		void insertDeath(int D);

		void insertSurv(tuple<int,int> S);

		vector<tuple<int,int>> getSurvs();

		void insertUnion(int C2, vector<int> C1);

		void insertSplits(int C1, int C2);

		vector<int>& allocBirths();

		///////////////////////////////////////////

		void showDeaths();

		void showSplits();

		void showBirths();

		void showSurvs();

		void showUnions();

};


#endif