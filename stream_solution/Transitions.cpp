#include "Transitions.h"

Transitions::Transitions(){};

////////////////////////////////////////////////////////////////////

void Transitions::insertDeath(int D){
	deaths.insert(deaths.end(), D);}

vector<int> Transitions::getDeaths(){
	return deaths;}

void Transitions::insertSurv(tuple<int,int> S){
	survs.insert(survs.begin(), S);}

vector<tuple<int,int>> Transitions::getSurvs(){
	return survs;}

void Transitions::insertUnion(int C2, vector<int> C1){
	unions[C2] = C1;}

unordered_map<int, vector<int>> Transitions::getUnions(){
	return unions;}

void Transitions::insertSplits(int C1, int C2){
	splits[C1].insert(splits[C1].end(), C2);}

unordered_map<int, vector<int>> Transitions::getSplits(){
	return splits;}

vector<int>& Transitions::allocBirths(){
	return births;}

vector<int> Transitions::getBirths(){
	return births;}

bool Transitions::checkExt(){

	if(splits.empty() == false || unions.empty() == false ||
		deaths.empty() == false || births.empty() == false){

		return true;
	}

	return false;

}

////////////////////////////////////////////////////////////////////


void Transitions::clear(){
	survs.clear();
	births.clear();
	deaths.clear();
	splits.clear();
	unions.clear();
}

void Transitions::showSurvs(){
	cout << "Sobreviventes" << endl;
	for(const auto &x : survs)cout << get<0>(x) << " -> " << get<1>(x)<< endl;
	cout << endl;
}

void Transitions::showUnions(){

	cout << "Uniões" << endl;
	for(const auto &x: unions){
		for(const auto &y: x.second) cout << y << " ";
		cout << "-> " << x.first << endl;		
	}
	cout << endl;
}

void Transitions::showBirths(){
	cout << "Nascimentos" << endl;
	for(const auto &x : births) cout << "BIRTHS -> " << x << endl;
	cout << endl;
}

void Transitions::showSplits(){
	
	cout << "Separações" << endl; 
	for(const auto &x: splits){
		cout << x.first << " -> ";
		for(const auto &y: x.second)cout << y << " ";
		cout << endl;
	}
	cout << endl;
}

void Transitions::showDeaths(){
	cout << "Mortes" << endl;
	for(const auto &x : deaths) cout << x << " -> DEATH " << endl;
	cout << endl;
}