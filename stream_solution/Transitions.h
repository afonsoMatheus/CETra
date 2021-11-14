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

		vector<tuple<T,T>> survs;
		unordered_map<T, vector<T>> unions;
		unordered_map<T, vector<T>> splits;
		vector<T> deaths;
		vector<T> births;

		unordered_map<T, vector<int>> interC;

	public:		

		Transitions();

		void clear();

		///////////////// GET/SET /////////////////

		void insertDeath(const T&);

		vector<T> getDeaths();

		void insertSurv(const tuple<T,T>&);

		vector<tuple<T,T>> getSurvs();

		void insertUnion(const T&, const vector<T>&);

		unordered_map<T, vector<T>> getUnions();

		void insertSplits(const T&, const T&);

		unordered_map<T, vector<T>> getSplits();

		vector<T>& allocBirths();

		vector<T> getBirths();

		void insertInterC(const T&, const int&);

		unordered_map<T, vector<int>> getInterC();

		///////////////// CHECKERS /////////////////

		bool checkExtChange();

		bool checkIntChange();

		////////////// VISUALIZATION //////////////

		void showDeaths();

		void showSplits();

		void showBirths();

		void showSurvs();

		void showUnions();

		void showInterC(const vector<string>&);

};

template <typename T>
Transitions<T>::Transitions(){};

template <typename T>
void Transitions<T>::clear(){
	survs.clear();
	births.clear();
	deaths.clear();
	splits.clear();
	unions.clear();
	interC.clear();
}

///////////////// GET/SET /////////////////

template <typename T>
void Transitions<T>::insertDeath(const T& D){
	deaths.insert(deaths.end(), D);
}

template <typename T>
vector<T> Transitions<T>::getDeaths(){
	return deaths;
}

template <typename T>
void Transitions<T>::insertSurv(const tuple<T,T>& S){
	survs.insert(survs.begin(), S);
}

template <typename T>
vector<tuple<T,T>> Transitions<T>::getSurvs(){
	return survs;
}

template <typename T>
void Transitions<T>::insertUnion( const T& C2, const vector<T>& C1){
	unions[C2] = C1;
}

template <typename T>
unordered_map<T, vector<T>> Transitions<T>::getUnions(){
	return unions;
}

template <typename T>
void Transitions<T>::insertSplits(const T& C1, const T& C2){
	splits[C1].insert(splits[C1].end(), C2);
}

template <typename T>
unordered_map<T, vector<T>> Transitions<T>::getSplits(){
	return splits;
}

template <typename T>
vector<T>& Transitions<T>::allocBirths(){
	return births;
}

template <typename T>
vector<T> Transitions<T>::getBirths(){
	return births;
}

template <typename T>
void Transitions<T>::insertInterC(const T& C1, const int& sta){
	interC[C1].insert(interC[C1].end(), sta);
}

template <typename T>
unordered_map<T, vector<int>> Transitions<T>::getInterC(){
	return interC;
}

///////////////// CHECKERS /////////////////

template <typename T>
bool Transitions<T>::checkExtChange(){

	if(!splits.empty() || !unions.empty() || !deaths.empty() || !births.empty()){
		return true;}
	return false;
}

template <typename T>
bool Transitions<T>::checkIntChange(){

	if(!interC.empty()) return true;
	return false;
}

////////////// VISUALIZATION //////////////

template <typename T>
void Transitions<T>::showSurvs(){
	cout << "- Sobreviventes" << endl;
	for(const auto &x : survs)cout << get<0>(x) << " -> " << get<1>(x)<< endl;
	cout << endl;
}

template <typename T>
void Transitions<T>::showUnions(){

	cout << "- Uniões" << endl;
	for(const auto &x: unions){
		for(const auto &y: x.second) cout << y << " ";
		cout << "-> " << x.first << endl;		
	}
	cout << endl;
}

template <typename T>
void Transitions<T>::showBirths(){
	cout << "- Nascimentos" << endl;
	for(const auto &x : births) cout << "BIRTHS -> " << x << endl;
	cout << endl;
}

template <typename T>
void Transitions<T>::showSplits(){
	
	cout << "- Separações" << endl; 
	for(const auto &x: splits){
		cout << x.first << " -> ";
		for(const auto &y: x.second)cout << y << " ";
		cout << endl;
	}
	cout << endl;
}

template <typename T>
void Transitions<T>::showDeaths(){
	cout << "- Mortes" << endl;
	for(const auto &x : deaths) cout << x << " -> DEATH " << endl;
	cout << endl;
}

template <typename T>
void Transitions<T>::showInterC(const vector<string>& nam){

	for (const auto &x: interC){
		cout << x.first << ": ";
		for(const auto &y: x.second){
			cout << nam[y] << " ";
		} 
		cout << endl;	
	} 
}


#endif