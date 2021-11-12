#include "Transitions.h"

template <typename T>
Transitions<T>::Transitions(){};

////////////////////////////////////////////////////////////////////

template <typename T>
void Transitions<T>::setNewRatio(float N){
	new_ratio = N;
}

template <typename T>
float Transitions<T>::getNewRatio(){
	return new_ratio;
}

template <typename T>
void Transitions<T>::setFailRatio(float N){
	fail_ratio = N;
}

template <typename T>
float Transitions<T>::getFailRatio(){
	return fail_ratio;
}

template <typename T>
void Transitions<T>::insertDeath(T D){
	deaths.insert(deaths.end(), D);}

template <typename T>
vector<T> Transitions<T>::getDeaths(){
	return deaths;}

template <typename T>
void Transitions<T>::insertSurv(tuple<T,T> S){
	survs.insert(survs.begin(), S);}

template <typename T>
vector<tuple<T,T>> Transitions<T>::getSurvs(){
	return survs;}

template <typename T>
void Transitions<T>::insertUnion(T C2, vector<T> C1){
	unions[C2] = C1;}

template <typename T>
unordered_map<T, vector<T>> Transitions<T>::getUnions(){
	return unions;}

template <typename T>
void Transitions<T>::insertSplits(T C1, T C2){
	splits[C1].insert(splits[C1].end(), C2);}

template <typename T>
unordered_map<T, vector<T>> Transitions<T>::getSplits(){
	return splits;}

template <typename T>
vector<T>& Transitions<T>::allocBirths(){
	return births;}

template <typename T>
vector<T> Transitions<T>::getBirths(){
	return births;}

template <typename T>
void Transitions<T>::insertInterC(T C1, int sta){
	interC[C1].insert(interC[C1].end(), sta);
}

template <typename T>
unordered_map<T, vector<int>> Transitions<T>::getInterC(){
	return interC;
}

template <typename T>
bool Transitions<T>::checkExtChange(){

	if(splits.empty() == false || unions.empty() == false ||
		deaths.empty() == false || births.empty() == false){

		return true;
	}

	return false;

}


template <typename T>
bool Transitions<T>::checkIntChange(){

	if(interC.empty() == false) return true;

	return false;

}

template <typename T>
bool Transitions<T>::checkNewRatio(float& lim){

	if(new_ratio > lim){
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////

template <typename T>
void Transitions<T>::clear(){
	survs.clear();
	births.clear();
	deaths.clear();
	splits.clear();
	unions.clear();
	interC.clear();
	new_ratio = 0;
	fail_ratio = 0;
}

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


template class Transitions<int>;
template class Transitions<char>;
