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
#include <fstream>

template <typename T>
class Transitions{

	/* Nickname */
	using overlaping = unordered_map<T, vector<float>>;
	using statistics = unordered_map<T, vector<float>>;

	private:

		/**************************** VARIABLES ****************************/

		/* Matrices */
		overlaping ext_matrix;
		statistics int_matrix;

		/* External variables */
		unordered_map<T, T> survs;
		unordered_map<T, vector<T>> unions;
		unordered_map<T, vector<T>> splits;
		vector<T> deaths;
		vector<T> births;

		/* Internal variable */
		unordered_map<T, vector<int>> interC;
		vector<string> interN;

		/* Fail/New samples variables */
		vector<T> faild;
		vector<T> newb;
		unordered_map<T, T> news;	

	public:	

		/**************************** SIGNATURES ****************************/

		Transitions();

		void clear();

		//------------- Get / Set -------------//

		void insertExtMatrix(const overlaping&);

		overlaping getExtMatrix();

		void insertIntMatrix(const statistics&);

		statistics getIntMatrix();

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

		unordered_map<T, vector<int>> getInterC();

		void insertInterN(const vector<string>&);

		void insertFDeath(const T&);

		vector<T> getFDeaths();

		void insertNBirth(const T&);

		vector<T> getNBirths();

		void insertNSurv(const T&, const T&);

		unordered_map<T, T> getNSurvs();

		void insertInterC(const T&, const int&);

		//------------- Checkers -------------//

		bool checkExtChange();

		bool checkIntChange();

		//---------- Visualization ----------//

		void showTransitions();

		void showDeaths();

		void showFDeaths();

		void showSplits();

		void showBirths();

		void showNBirths();

		void showSurvs();

		void showUnions();

		void showInterC();

		void showNStatistic();

		void exportTrans(const string &path){

			if(survs.empty() && !checkExtChange()) return;

			unordered_map<T, int> hm_x;
			unordered_map<T, int> hm_y;
			unordered_map<string, int> dea;

			vector<T> source;
			vector<T> target;
			vector<float> value;
			vector<string> labels;

			int l = 0;

			if(!survs.empty()){
				for(const auto &x : survs){
					source.push_back(l++);
					hm_x[get<0>(x)] = source.back();
					labels.push_back("C" + to_string(get<0>(x)));

					target.push_back(l++);
					hm_y[get<1>(x)] = target.back();
					labels.push_back("C" + to_string(get<1>(x)));

					value.push_back(1);

				}
			}

			if(!splits.empty()){
				for(const auto &x: splits){
					for(const auto &y: x.second){
						
						if(hm_x.find(x.first) == hm_x.end()){
							source.push_back(l++);
							hm_x[x.first] = source.back();
							labels.push_back("C" + to_string(x.first));
						}else{
							source.push_back(hm_x[x.first]);
						}
						
						if(hm_y.find(y) == hm_y.end()){
							target.push_back(l++);
							hm_y[y] = target.back();
							labels.push_back("C" + to_string(y));
						}else{
							target.push_back(hm_y[y]);
						}

						value.push_back(1);	
					}
					
				}
			}

			if(!unions.empty()){
				for(const auto &x: unions){
					for(const auto &y: x.second){ 
						
						if(hm_y.find(x.first) == hm_y.end()){
							target.push_back(l++);
							hm_y[x.first] = target.back();
							labels.push_back("C" + to_string(x.first));
						}else{
							target.push_back(hm_y[x.first]);
						}
						
						if(hm_x.find(y) == hm_x.end()){
							source.push_back(l++);
							hm_x[y] = source.back();
							labels.push_back("C" + to_string(y));
						}else{
							source.push_back(hm_x[y]);
						}		

						value.push_back(1);						
					}
					
				}
			}
			 
			if(!births.empty()){
				for(const auto &x : births){
					if(hm_y.find(x) == hm_y.end()){
						target.push_back(l++);
						hm_y[x] = target.back();
						labels.push_back("C" + to_string(x));
					}else{
						target.push_back(hm_y[x]);
					}

					if(dea.find("BIRTH") == dea.end()){
						source.push_back(l++);
						dea["BIRTH"] = source.back();
						labels.push_back("BIRTH");
					}else{
						source.push_back(dea["BIRTH"]);
					}

					value.push_back(1);	
				}

			}

			if(!deaths.empty()){
				for(const auto &x : deaths){
					if(hm_x.find(x) == hm_x.end()){
						source.push_back(l++);
						hm_x[x] = source.back();
						labels.push_back("C" + to_string(x));
					}else{
						source.push_back(hm_x[x]);
					}

					if(dea.find("DEATH") == dea.end()){
						target.push_back(l++);
						dea["DEATH"] = target.back();
						labels.push_back("DEATH");
					}else{
						target.push_back(dea["DEATH"]);
					}

					value.push_back(1);	
				}

			}

			ofstream output(path);

			for(const auto &x: source){ 
				output << x << " ";
			}	
			output << endl;

			for(const auto &x: target){ 
				output << x << " ";
			}	
			output << endl;

			for(const auto &x: value){ 
				output << x << " ";
			}	
			output << endl;

			for(const auto &x: labels){ 
				output << x << " ";
			}

			output.close();

		}

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
	interN.clear();
	faild.clear();
	newb.clear();
	news.clear();

}

//------------------ Get / Set ------------------//

/*
*	Func: 		
*		insertExtMatrix(const overlaping&)
*	Args: 
*		A overlapping matrix. 
*	Ret:
*		None, inserts the transitions overlapping matrix.
*/
template <typename T>
void Transitions<T>::insertExtMatrix(const overlaping& mat){
	ext_matrix = mat;
}

/*
*	Func: 		
*		getExtMatrix()
*	Args: 
*		None. 
*	Ret:
*		None, returns the transitions overlapping matrix.
*/
template <typename T>
auto Transitions<T>::getExtMatrix() -> overlaping{
	return ext_matrix;
}

/*
*	Func: 		
*		insertIntMatrix(const statistics&)
*	Args: 
*		A statistics matrix. 
*	Ret:
*		None, inserts the transitions statistics matrix.
*/
template <typename T>
void Transitions<T>::insertIntMatrix(const statistics& mat){
	int_matrix = mat;
}

/*
*	Func: 		
*		insertIntMatrix(const statistics&)
*	Args: 
*		A statistics matrix. 
*	Ret:
*		None, inserts the transitions statistics matrix.
*/
template <typename T>
auto Transitions<T>::getIntMatrix() -> statistics{
	return int_matrix;
}

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
*		Array, array with the detected dead clusters.
*/
template <typename T>
vector<T> Transitions<T>::getDeaths(){
	return deaths;
}

/*
*	Func: 		
*		insertSurv(const T&, const T&)
*	Args: 
*		A survived referential cluster,
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
*		Hash Table, hash tables with the detected survives.
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
*		Arrays with the corresponding referential clusters. 
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
*		Hash Table, hash tables with the detected unions.
*/
template <typename T>
unordered_map<T, vector<T>> Transitions<T>::getUnions(){
	return unions;
}

/*
*	Func: 		
*		insertSplits(const T&, const T&)
*	Args: 
*		A referential cluster,
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
*		Hash Table, hash tables with the detected splits.
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
*		A referential cluster,
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
*		insertInterN(const vector<string>&)
*	Args: 
*		Array with the internal statistics names. 
*	Ret:
*		None, assign the statistics names.
*/
template <typename T>
void Transitions<T>::insertInterN(const vector<string> & nams){
	interN = nams;
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
*		showTransitions()
*	Args: 
*		None. 
*	Ret:
*		None, show external and internal transitions, if any.
*/
template <typename T>
void Transitions<T>::showTransitions(){
	
	if(checkExtChange()){

		cout << "+ EXT TRANSITIONS" << endl << endl; 

		showSplits();

		showUnions();
		 
		showDeaths();
		showFDeaths();
		
		showBirths();
		showNBirths();
		
	}

	if(checkIntChange()){

		cout << "+ INT TRANSITIONS" << endl << endl;

		showInterC();
		showNStatistic();

		cout << endl;
	}
}

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
	
	if(survs.empty()) return;

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

	if(unions.empty()) return;

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

	if(splits.empty()) return;
	
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

	if(deaths.empty()) return;

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

	if(faild.empty()) return;

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

	if(births.empty()) return;

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

	if(newb.empty()) return;

	for(const auto &x: newb){
		cout << x << " is birth by new sensors" << endl;
	}
	cout<<endl;
}

/*
*	Func: 		
*		showInterC(const vector<string>&)
*	Args: 
*		None.
*	Ret:
*		None, show detected internal statistics.
*/
template <typename T>
void Transitions<T>::showInterC(){

	if(interC.empty()) return;

	for (const auto &x: interC){
		cout << x.first << " -> " << survs[x.first] << ": ";
		for(const auto &y: x.second){
			cout << interN[y] << " ";
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