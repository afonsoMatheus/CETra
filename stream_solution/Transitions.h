#ifndef TRANSITIONS_H
#define TRANSITIONS_H 

#include <vector>
#include <tuple>
#include <unordered_map>
#include <iostream>

using namespace std;

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