/* Author: Afonso Matheus                                                     */
/* Date: 2021                                                                 */
/* Ver. Beta                                                                  */
//----------------------------------------------------------------------------//
//                                                                            //                                                                          //
// Script that contains the implementation of Transitions class used by the   // 
// TraCES algorithm.                                                          //
//                                                                            //
//----------------------------------------------------------------------------//

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

		/**************************** VARIABLES ****************************/

		/* External variables */

		unordered_map<T, T> survs;
		unordered_map<T, vector<T>> unions;
		unordered_map<T, vector<T>> splits;
		vector<T> deaths;
		vector<T> births;

		/* Internal variable */

		unordered_map<T, vector<int>> interC;

		/* Fail/New samples variables */

		vector<T> faild;
		vector<T> newb;
		unordered_map<T, T> news;	

	public:	

		/**************************** SIGNATURES ****************************/

		Transitions();

		void clear();

		//------------- Get / Set -------------//

		void insertDeath(const T&);

		vector<T> getDeaths();

		void insertSurv(const T&, const T&);

		unordered_map<T, T> getSurvs();

		void insertUnion(const T&, const vector<T>&);

		unordered_map<T, vector<T>> getUnions();

		void insertSplits(const T&, const T&);

		unordered_map<T, vector<T>> getSplits();

		vector<T>& allocBirths();

		vector<T> getBirths();

		void insertInterC(const T&, const int&);

		unordered_map<T, vector<int>> getInterC();

		void insertFDeath(const T&);

		vector<T> getFDeaths();

		void insertNBirth(const T&);

		vector<T> getNBirths();

		void insertNSurv(const T&, const T&);

		unordered_map<T, T> getNSurvs();

		//------------- Checkers -------------//

		bool checkExtChange();

		bool checkIntChange();

		//---------- Visualization ----------//

		void showDeaths();

		void showFDeaths();

		void showSplits();

		void showBirths();

		void showNBirths();

		void showSurvs();

		void showUnions();

		void showInterC(const vector<string>&);

		void showNStatistic();

};

/************************** IMPLEMENTATIONS **************************/

/*
*	Func: 		
*		Transitions() *Constructor
*	Args: 
*		None. 
*	Ret:
*		None, constructs a Transitions object.
*/
template <typename T>
Transitions<T>::Transitions(){};

/*
*	Func: 		
*		clear()
*	Args: 
*		None. 
*	Ret:
*		None, clear all Transitions variables.
*/
template <typename T>
void Transitions<T>::clear(){
	survs.clear();
	births.clear();
	deaths.clear();
	splits.clear();
	unions.clear();
	interC.clear();
	faild.clear();
	newb.clear();
	news.clear();

}

//------------------ Get / Set ------------------//

/*
*	Func: 		
*		insertDeath(const T&)
*	Args: 
*		A dead cluster. 
*	Ret:
*		None, inserts a cluster label in the deaths array.
*/
template <typename T>
void Transitions<T>::insertDeath(const T& cr){
	deaths.insert(deaths.end(), cr);
}

/*
*	Func: 		
*		getDeaths()
*	Args: 
*		None. 
*	Ret:
*		Array, array with the detecteds deads cluster.
*/
template <typename T>
vector<T> Transitions<T>::getDeaths(){
	return deaths;
}

/*
*	Func: 		
*		insertSurv(const T&, const T&)
*	Args: 
*		A survived referencial cluster,
*		The corresponding evolutionary cluster. 
*	Ret:
*		None, inserts a survive in the survives hash table.
*/
template <typename T>
void Transitions<T>::insertSurv(const T& cr, const T& ce){
	survs[cr] = ce;
}

/*
*	Func: 		
*		getSurvs()
*	Args: 
*		None. 
*	Ret:
*		Hash Table, hash tables with the detecteds survives.
*/
template <typename T>
unordered_map<T, T> Transitions<T>::getSurvs(){
	return survs;
}

/*
*	Func: 		
*		insertUnion(const T&, const vector<T>&)
*	Args: 
*		An evolutionary cluster,
*		Arrays with the correspondings referencial clusters. 
*	Ret:
*		None, inserts an union in the unions hash table.
*/
template <typename T>
void Transitions<T>::insertUnion( const T& ce, const vector<T>& crs){
	unions[ce] = crs;
}

/*
*	Func: 		
*		getUnions()
*	Args: 
*		None. 
*	Ret:
*		Hash Table, hash tables with the detecteds unions.
*/
template <typename T>
unordered_map<T, vector<T>> Transitions<T>::getUnions(){
	return unions;
}

/*
*	Func: 		
*		insertSplits(const T&, const T&)
*	Args: 
*		A referencial cluster,
*		An evolutionary cluster. 
*	Ret:
*		None, inserts a split part in the splits hash table.
*/
template <typename T>
void Transitions<T>::insertSplits(const T& cr, const T& ce){
	splits[cr].insert(splits[cr].end(), ce);
}

/*
*	Func: 		
*		getSplits()
*	Args: 
*		None. 
*	Ret:
*		Hash Table, hash tables with the detecteds splits.
*/
template <typename T>
unordered_map<T, vector<T>> Transitions<T>::getSplits(){
	return splits;
}

/*
*	Func: 		
*		allocBirths()
*	Args: 
*		None. 
*	Ret:
*		Array reference, reference to the detected births array.
*/
template <typename T>
vector<T>& Transitions<T>::allocBirths(){
	return births;
}

/*
*	Func: 		
*		getBirths()
*	Args: 
*		None. 
*	Ret:
*		Array, array with the detected births.
*/
template <typename T>
vector<T> Transitions<T>::getBirths(){
	return births;
}

/*
*	Func: 		
*		insertInterC(const T&, const int&)
*	Args: 
*		A referencial cluster,
*		An index of the corresponding statistic.
*	Ret:
*		None, inserts a statistic index in a hash table.
*/
template <typename T>
void Transitions<T>::insertInterC(const T& cr, const int& sta){
	interC[cr].insert(interC[cr].end(), sta);
}

/*
*	Func: 		
*		getInterC()
*	Args: 
*		None. 
*	Ret:
*		Hash Table, the internal statistics hash table.
*/
template <typename T>
unordered_map<T, vector<int>> Transitions<T>::getInterC(){
	return interC;
}

/*
*	Func: 		
*		insertFDeath(const T&)
*	Args: 
*		A dead cluster. 
*	Ret:
*		None, inserts a dead cluster in the failing cluster array.
*/
template <typename T>
void Transitions<T>::insertFDeath(const T& cr){
	faild.insert(faild.end(), cr);
}

/*
*	Func: 		
*		getFDeaths()
*	Args: 
*		None. 
*	Ret:
*		Array, the array with the clusters deaths by failing sensors.
*/
template <typename T>
vector<T> Transitions<T>::getFDeaths(){
	return faild;
}

/*
*	Func: 		
*		insertNBirth(const T&)
*	Args: 
*		A birth cluster. 
*	Ret:
*		None, inserts a birth cluster in the new cluster by new sensors array.
*/
template <typename T>
void Transitions<T>::insertNBirth(const T& ce){
	newb.insert(newb.end(), ce);
}

/*
*	Func: 		
*		getNBirths()
*	Args: 
*		None. 
*	Ret:
*		Array, the array with the clusters births by newer sensors.
*/
template <typename T>
vector<T> Transitions<T>::getNBirths(){
	return newb;
}

/*
*	Func: 		
*		insertNSurv(const T&, const T&)
*	Args: 
*		A reference cluster,
*		An evolutionary cluster. 
*	Ret:
*		None, inserts a survive that has many sensors between clusters in a hash table.
*/
template <typename T>
void Transitions<T>::insertNSurv(const T& cr, const T& ce){
	news[cr] = ce;
}

/*
*	Func: 		
*		getNSurvs()
*	Args: 
*		None. 
*	Ret:
*		Hash Table, the survived clusters with has many sensors between clusters.
*/
template <typename T>
unordered_map<T,T> Transitions<T>::getNSurvs(){
	return news;
}

//------------------ Checkers ------------------//

/*
*	Func: 		
*		checkExtChange()
*	Args: 
*		None. 
*	Ret:
*		Bool, returns true if any of the external variables is not empty.
*/
template <typename T>
bool Transitions<T>::checkExtChange(){

	if(!splits.empty() || !unions.empty() || !deaths.empty() || !births.empty()){
		return true;}
	return false;
}

/*
*	Func: 		
*		checkIntChange()
*	Args: 
*		None. 
*	Ret:
*		Bool, returns true if the internal variable is not empty.
*/
template <typename T>
bool Transitions<T>::checkIntChange(){

	if(!interC.empty()) return true;
	return false;
}

//--------------- Visualization ---------------//

/*
*	Func: 		
*		showSurvs()
*	Args: 
*		None. 
*	Ret:
*		None, show detected survives.
*/
template <typename T>
void Transitions<T>::showSurvs(){
	cout << "- Sobreviventes" << endl;
	for(const auto &x : survs)cout << get<0>(x) << " -> " << get<1>(x)<< endl;
	cout << endl;
}

/*
*	Func: 		
*		showUnions()
*	Args: 
*		None. 
*	Ret:
*		None, show detected unions.
*/
template <typename T>
void Transitions<T>::showUnions(){

	cout << "- Uniões" << endl;
	for(const auto &x: unions){
		for(const auto &y: x.second) cout << y << " ";
		cout << "-> " << x.first << endl;		
	}
	cout << endl;
}

/*
*	Func: 		
*		showSplits()
*	Args: 
*		None. 
*	Ret:
*		None, show detected splits.
*/
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

/*
*	Func: 		
*		showDeaths()
*	Args: 
*		None. 
*	Ret:
*		None, show detected deaths.
*/
template <typename T>
void Transitions<T>::showDeaths(){
	cout << "- Mortes" << endl;
	for(const auto &x : deaths) cout << x << " -> DEATH " << endl;
	cout << endl;
}

/*
*	Func: 		
*		showFDeaths()
*	Args: 
*		None. 
*	Ret:
*		None, show if death was caused by missing sensors.
*/
template <typename T>
void Transitions<T>::showFDeaths(){

	for(const auto &x: faild){
		cout << x << " is dead by missing sensors" << endl;
	}
	cout<<endl;
}

/*
*	Func: 		
*		showBirths()
*	Args: 
*		None. 
*	Ret:
*		None, show detected births.
*/
template <typename T>
void Transitions<T>::showBirths(){
	cout << "- Nascimentos" << endl;
	for(const auto &x : births) cout << "BIRTHS -> " << x << endl;
	cout << endl;
}

/*
*	Func: 		
*		showNBirths()
*	Args: 
*		None. 
*	Ret:
*		None, show if birth was caused by new sensors.
*/
template <typename T>
void Transitions<T>::showNBirths(){

	for(const auto &x: newb){
		cout << x << " is birth by new sensors" << endl;
	}
	cout<<endl;
}

/*
*	Func: 		
*		showInterC(const vector<string>&)
*	Args: 
*		Array with the statistics names.
*	Ret:
*		None, show detected internal statistics.
*/
template <typename T>
void Transitions<T>::showInterC(const vector<string>& nam){

	for (const auto &x: interC){
		cout << x.first << " -> " << survs[x.first] << ": ";
		for(const auto &y: x.second){
			cout << nam[y] << " ";
		} 
		cout << endl;	
	} 
}

/*
*	Func: 		
*		showNStatistic()
*	Args: 
*		None. 
*	Ret:
*		None, show a significant size increase by new sensors in a survive.
*/
template <typename T>
void Transitions<T>::showNStatistic(){

	for(const auto &x : news){
		cout << "Too much new sensors between ref cluster " << x.first << " and evo cluster " << x.second << endl; 
	}

}

#endif