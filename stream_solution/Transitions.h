#ifndef TRANSITIONS_H
#define TRANSITIONS_H

using namespace std;

#include <vector>
#include <tuple>
#include <string>
#include <unordered_map>
#include <iostream>

template <typename T>
class Transitions{

	private:

		float new_ratio = 0;
		float fail_ratio = 0;

		vector<T> news_clu;
		//vector<T> fails_clu;

		vector<tuple<T,T>> survs;
		unordered_map<T, vector<T>> unions;
		unordered_map<T, vector<T>> splits;
		vector<T> deaths;
		vector<T> births;

		unordered_map<T, vector<int>> interC;

	public:		

		Transitions();

		void clear();

		void setNovelClusters(T N){
			news_clu.insert(news_clu.end(), N);
		}

		vector<T> getNovelClusters(){
			return news_clu;
		}

		/*void setFailClusters(T N){
			fails_clu.insert(news_clu.end(), N);
		}

		vector<T> getFailClusters(){
			return fails_clu;
		}*/

		///////////////////////////////////////////

		void setNewRatio(float N);

		float getNewRatio();

		void setFailRatio(float N);

		float getFailRatio();

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

		void insertInterC(T C1, int sta);

		unordered_map<T, vector<int>> getInterC();

		bool checkExtChange();

		bool checkIntChange();

		bool checkNewRatio(float &);

		bool checkNovelClusters(){
			if(news_clu.empty() == false) return true;
			return false;
		}

	/*	bool checkFailClusters(){
			if(fails_clu.empty() == false) return true;
			return false;
		}*/

		///////////////////////////////////////////

		void showDeaths();

		void showSplits();

		void showBirths();

		void showSurvs();

		void showUnions();

		void showInterC(const vector<string>&);

};


#endif