/* Author: Afonso Matheus                                                     */
/* Date: 2021                                                                 */
/* Ver. Beta                                                                  */
//----------------------------------------------------------------------------//
//                                                                            //                                                                          //
// Script that contains the implementation of cluster's transition detection  // 
// between clusterings made by both conventional and data streams algorithms. //
//                                                                            //
//----------------------------------------------------------------------------//

#ifndef CETRA_H
#define CETRA_H

#include <unordered_map>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_set>
#include <deque>
#include <exception>
#include <cmath>
#include <string>
#include "Transitions.h"

using namespace std;

/* Sensors and clusters labels can be of must primary types, int is the default for both */
template <typename S = int, typename C = int>
class CETra{

	/* Nicknames for code simplification */
	using clustering = unordered_map<C, vector<S> >;
	using overlaping = unordered_map<C, vector<float>>;
	using search_table = unordered_map<S, tuple<C, float>>;
	using statistics = unordered_map<C, vector<float>>;

	private:

		/**************************** VARIABLES ****************************/

		/* Constructor variables */
		int winSize;
		vector<float> limits;
		vector<string> names;
		float surv_limit;
		float split_limit;

		/* Referential Clustering variables */
		clustering clusR;
		unordered_map<C,float> refW;
		statistics staR;
		unsigned int refRate;

		/* Evolutionary Clustering variables */
		statistics staE;
		set<C> evoLabels;

		/* Fail/New sensors variables */
		unordered_map<C, vector<S>> failS;
		unordered_set<S> ns;
		unordered_map<C, vector<S>> newS;

		/* Statistics queue */
		deque<statistics> staQueue;

		/**************************** SIGNATURES ****************************/

		//---------- Ref. Clustering Methods ----------//

		void storeClustering(const vector<S>&, const vector<C>&, const vector<float>&);

		template <class T>
		void storeRefStatistics(initializer_list<T>);

		void freeRef();

		//---------- Evo. Clustering Methods ----------//

		search_table getEvoInformation(const vector<S>&, const vector<C>&, const vector<float>&);

		template <class T>
		void storeEvoStatistics(initializer_list<T>);

		void freeEvo();

		//--------- Trans. Detection Methods ---------//

		Transitions<C> trackTransitions(search_table&);

		overlaping clusterOverlap(search_table&);
		
		unordered_map<C,int> useLabels();

		//------------- Ext. Transitions -------------//

		void extTransitions(Transitions<C>&);

		float sumSplits(const vector<float>&, const vector<C>&);

		unordered_map<int,C> hashLabels();

		void checkFailDeath(const C &, Transitions<C>&);

		void checkNewBirth(Transitions<C>&);

		//------------- Int. Transitions -------------//
		
		void intTransitions(Transitions<C>&);

		void checkIntTrans(Transitions<C>&);

		void checkNewStatistic(const C &, const C &, Transitions<C>&);

		//--------------- Visualization ---------------//

		void showRefClustering();

		void showStatistics();

		void showIntersection(const overlaping&);

		void showOverlaping(const overlaping&);

		void showTrackStatistics();

		void showTransitions(Transitions<C>&);

		void showIntQueue();

		void showRefRate(){
			cout << "Survival Rate: " << refRate << endl;
		}

	public:

		CETra(const int = 3);

		//----------- Execution -----------//

		Transitions<C> execute(const vector<S>&, const vector<C> &, const vector<float> &);

		template <class T>
		Transitions<C> execute(const vector<S>&, const vector<C> &, const vector<float> &, initializer_list<T>);

		//---------- User Configs ----------//

		void configSizeLimit(const float&);

		void configNewIntStatistic(const float&, const string&);

		void configSurvLimit(const float&);

		void configSplitLimit(const float&);
	
};

/************************** IMPLEMENTATIONS **************************/

/*
*
*	Func: 		
*		CETra(const int, const vector<float>) *Constructor
*	Args: 
*		Size of statistics window, 
*		Limits of the internal statistics.
*	Ret:
*		None, constructs a CETra object. 
*
*/
template <typename S, typename C>
CETra<S, C>::CETra(const int N){

	winSize = N;

	limits.insert(limits.begin(), 0.5);

	names.insert(names.begin(), "Sizes");

	surv_limit = 0.5;
	split_limit = surv_limit/2;

}

/*
*
*	Func: 		
*		execute(const vector<S> &, const vector<C> &, const vector<float> &)
*	Args: 
*		Array with the clustering sensors labels, 
*		Array with the clustering clusters labels,
*		Array with the clustering weights values.
*	Ret:
*		Transitions class, executes the transitions detection algorithm with only the size 
*		internal statistic and returns it. 
*
*/
template <typename S, typename C>
Transitions<C> CETra<S, C>::execute(const vector<S> &sen, const vector<C> &clu, const vector<float> &wei){
	
	try{

		/* The method must not be executed if the sensors/clusters/weights arrays don't have the same sizes */
		if(sen.size() != clu.size() or sen.size() != wei.size()){
			throw invalid_argument("ERROR: The sensors/clusters/weights arrays are not the same size!");
		}

		/* Command for first execution, when there is no referential clustering */
		if(clusR.empty()){

			/* Storing the current clustering and it's clusters weights */
			storeClustering(sen, clu, wei);

			/* Storing the clusters sizes statistics*/
			for (const auto &x: clusR) 
				staR[x.first].insert(staR[x.first].end(), x.second.size());

			refRate = 1;

			return Transitions<C>();

		}else{

			//cout << endl << "///// NEXT WINDOW /////" << endl << endl;

			/* Getting all necessary evolutionary clustering variables, including the evolved clusters sizes */
			search_table evoTable = getEvoInformation(sen, clu, wei);

			/* Tracking the transitions between the current referential clustering and the current evolutionary clustering */
			Transitions<C> TRANS = trackTransitions(evoTable);

			/* Showing survived clusters, if any */
			//if(!TRANS.getSurvs().empty()) TRANS.showSurvs();

			/* Verifying if there was any detected internal or external transitions */
			if(TRANS.checkExtChange() || TRANS.checkIntChange()){

				//cout << "=== TRANS DETECTED ===" << endl << endl;

				/* Showing the detected internal or/and external transitions */
				//TRANS.showTransitions();

				//cout << "### UPT REF CLUSTERING ###" << endl << endl;				

				/* Updating the referential clustering variables with the current evolutionary clustering */
				freeRef();
				storeClustering(sen, clu, wei);
				staR = staE;
			}

			/* Freeing the evolutionary clustering variables */
			freeEvo();

			return TRANS;
		}

	}catch(invalid_argument& e){

		cerr << e.what() << endl;

		throw -1;

	}

}

/*
*
*	Func: 		
*		execute(const vector<S> &, const vector<C> &, const vector<float> &, initializer_list<T>)
*	Args: 
*		Array with the clustering sensors labels, 
*		Array with the clustering clusters labels,
*		Array with the clustering weights values,
*		List with the desired internal clusters statistics.
*	Ret:
*		Transitions class, executes the transition detection algorithm with the desired
*		internal clusters statistics and returns it.  
*
*/
template <typename S, typename C>
template <class T>
Transitions<C> CETra<S,C>::execute(const vector<S> &sen, const vector<C> &clu, const vector<float> &wei, initializer_list<T> list){

	try{

		/* The method must not be executed if the sensors/clusters/weights arrays don't have the same sizes */
		if(sen.size() != clu.size() or sen.size() != wei.size()){
			throw invalid_argument("ERROR: The sensors/clusters/weights arrays are not the same size!");
		}

		/* Inserting default limits for missing statistics limits in input */
		if(limits.size() < list.size() + 1){
			for(auto i = limits.size(); i < list.size() + 1; i++){
				limits.insert(limits.end(), 0.5);
			}
		}

		/* Inserting default statistics names for missing statistics names in input */
		if(names.size() < list.size() + 1){
			int j = 1;
			for(auto i = names.size(); i < list.size() + 1; i++){
				names.insert(names.end(), "Sta_" + to_string(j));
				j++;
			}
		}

		/* Command for first execution, when there is no referential clustering */
		if(clusR.empty()){

			/* Storing the current clustering and it's clusters weights */
			storeClustering(sen, clu, wei);

			/* Storing the clusters sizes plus the internal statistics inputted */
			storeRefStatistics(list);

			refRate = 1;

			return Transitions<C>();

		}else{

			//cout << endl << "/////////////// NEXT WINDOW ///////////////" << endl << endl;

			/* Getting all necessary evolutionary clustering variables, including the evolved clusters sizes */
			search_table evoTable = getEvoInformation(sen, clu, wei);

			/* Storing the internal statistics inputted */
			storeEvoStatistics(list);

			/* Monitoring the transitions between the current referential clustering and the current evolutionary clustering */
			Transitions<C> TRANS = trackTransitions(evoTable);

			/* Showing survived clusters, if any */
			if(!TRANS.getSurvs().empty()) TRANS.showSurvs();

			/* Verifying if there was any detected internal or external transitions */
			if(TRANS.checkExtChange() || TRANS.checkIntChange()){

				//cout << "=== TRANS DETECTED ===" << endl << endl;

				/* Showing the detected internal or/and external transitions */
				TRANS.showTransitions();

				//cout << "### UPT REF CLUSTERING ###" << endl << endl;

				/* Updating the referential clustering variables with the current evolutionary clustering */
				freeRef();
				storeClustering(sen, clu, wei);
				staR = staE;

				refRate = 0;

			}

			/* Freeing the evolutionary clustering variables */
			freeEvo();

			refRate++;

			showRefRate();

			return TRANS;
		}

	}catch(invalid_argument& e){

		cerr << e.what() << endl;

		throw -1;
	}

}

//------------------------------- User Configs -------------------------------//

/*
*	Func: 		
*		configSizeLimit(const float&)
*	Args: 
*		Limit for size change. (0.0 - 1.0) 
*	Ret:
*		None, updates the size limit in the internal limits array.  
*/
template <typename S, typename C>
void CETra<S,C>::configSizeLimit(const float& sl){
	if(sl < 0.0 || sl > 1.0) 
		throw invalid_argument("ERROR: The size limit must be between 0.0 and 1.0");
	limits[0] = sl;
}

/*
*	Func: 		
*		configNewIntStatistic(const string&, const float&)
*	Args: 
*		Limit of the internal statistic,
*		Name of the internal statistic. 
*	Ret:
*		None, updates the names and limits arrays.  
*/
template <typename S, typename C>
void CETra<S,C>::configNewIntStatistic(const float& lim, const string& nam){

	if(lim < 0.0 || lim > 1.0) throw invalid_argument("ERROR: The limits must be between 0.0 and 1.0"); 
	limits.insert(limits.end(), lim);
	names.insert(names.end(), nam);
	
}

/*
*	Func: 		
*		configSurvLimit(const float&)
*	Args: 
*		Limit for survival change. *(0.0 - 1.0) *(survival > split)  
*	Ret:
*		None, updates the survival limit in the internal survival limit variable.  
*/
template <typename S, typename C>
void CETra<S,C>::configSurvLimit(const float& ls){
	if(ls < 0.0 || ls > 1.0) 
		throw invalid_argument("ERROR: The survival limit must be between 0.0 and 1.0");
	if(ls < split_limit)
		throw invalid_argument("ERROR: The survival limit must be higher than the current split limit: "+ to_string(split_limit));
	surv_limit = ls;
}

/*
*	Func: 		
*		configSplitLimit(const float&)
*	Args: 
*		Limit for split change. *(0.0 - 1.0) *(split < survival) 
*	Ret:
*		None, updates the split limit in the internal split limit variable.  
*/
template <typename S, typename C>
void CETra<S,C>::configSplitLimit(const float& ls){
	if(ls < 0.0 || ls > 1.0) 
		throw invalid_argument("ERROR: The split limit must be between 0.0 and 1.0");
	if(ls >= surv_limit){
		throw invalid_argument("ERROR: The split limit must be lower than the current survival limit: " + to_string(surv_limit));
	} 
	split_limit = ls;
}

//------------------------------- Ref. Clustering Implementations -------------------------------//

/*
*	Func: 		
*		storeClustering(const vector<S> &, const vector<C> &, const vector<float> &)
*	Args: 
*		Array with the clustering sensors labels, 
*		Array with the clustering clusters labels,
*		Array with the clustering weights values.
*	Ret:
*		None, stores the referential clustering and it's clusters weights in an internal
*		hash table each. 
*/
template <typename S, typename C>
void CETra<S, C>::storeClustering(const vector<S> &sen, const vector<C> &clu, const vector<float> &wei){

	/* Storing referencial clustering clusters and it's clusters weights */
	for (int i = 0; i < sen.size(); ++i){
		clusR[clu[i]].insert(clusR[clu[i]].end(), sen[i]);
		refW[clu[i]] += wei[i];
	}

	//showRefClustering();
}

/*
*	Func: 		
*		storeRefStatistics(initializer_list<T>)
*	Args: 
*		List with all the internal statistics. 
*	Ret:
*		None, stores the inputted referential clusters internal statistics in a hash table. 
*/
template <typename S, typename C>
template <class T>
void CETra<S,C>::storeRefStatistics(initializer_list<T> list){

	/* The set structure helps to detect the clustering labels maintaining ordering */
	set<C> ref_labels;

	/* Storing referential clustering cluster's sizes */
	for (const auto &x: clusR){
		if(ref_labels.find(x.first) == ref_labels.end()) ref_labels.insert(x.first);
		staR[x.first].insert(staR[x.first].end(), x.second.size()); 
	}

	/* Storing referential clustering cluster's internal statistics */
	for(auto &s: list){
		auto itr = ref_labels.begin();

		/* Abort if the detected labels size are different from the inputed statistics size */
		if(s.size() != ref_labels.size()){
			throw invalid_argument("ERROR: The labels/statistics arrays are not the same size!");
		}

		for(const auto &x: s){
			staR[*itr].insert(staR[*itr].end(), x);
			itr++;
		}		 
	}	

}

/*
*	Func: 		
*		freeRef()
*	Args: 
*		None 
*	Ret:
*		None, empty the referential clustering variables. 
*/
template <typename S, typename C>
void CETra<S,C>::freeRef(){

	clusR.clear();
	refW.clear();
	staR.clear();
	staQueue = {};

}

//------------------------------- Evo. Clustering Implementations -------------------------------//

/*
*	Func: 		
*		getEvoInformation(const vector<S> &, const vector<C> &, const vector<float> &)
*	Args: 
*		Array with the clustering sensors labels, 
*		Array with the clustering clusters labels,
*		Array with the clustering weights values,
*	Ret:
*		Search Table, build all the necessary evolutionary information, such as the evolutionary labels,
*		the search table and the clusters size internal statistic. 
*/
template <typename S, typename C>
auto CETra<S,C>::getEvoInformation(const vector<S> &sen, const vector<C> &clu, const vector<float> &wei) -> search_table{

	search_table evoTable;

	for (int i = 0; i < sen.size(); ++i){

		/* Getting evolutionary clustering cluster's labels */
		if(evoLabels.find(clu[i]) == evoLabels.end()) 
			evoLabels.insert(clu[i]);

		/* Building evolutionary clustering search table */
		evoTable[sen[i]] = make_tuple(clu[i],wei[i]);

		/* Building evolutionary clustering cluster's sizes */
		if(staE.find(clu[i]) == staE.end())
			staE[clu[i]].insert(staE[clu[i]].end(), 0);
		staE[clu[i]][0] += 1;

		/* Copying evolutionary clustering sensors in an auxiliary structure for new sensor detection */
		ns.insert(sen[i]);
	}

	return evoTable;

}

/*
*	Func: 		
*		storeEvoStatistics(initializer_list<T>)
*	Args: 
*		List with all the internal statistics.
*	Ret:
*		None, stores the inputted evolutionary clusters internal statistics in a hash table. 
*/
template <typename S, typename C>
template <class T>
void CETra<S,C>::storeEvoStatistics(initializer_list<T> list){

	/* Storing evolutionary clustering cluster's internal statistics */
	for(const auto &s: list){
		auto itr = evoLabels.begin();

		/* Abort if the previously detected evo labels size are different from the inputted statistics size */
		if(s.size() != evoLabels.size()){
			throw invalid_argument("ERROR: The labels/statistics arrays are not the same size!");
		}

		for(const auto &x: s){
			staE[*itr].insert(staE[*itr].end(), x);
			itr++;
		}		 
	}		
}

/*
*	Func: 		
*		freeEvo()
*	Args: 
*		None. 
*	Ret:
*		None, empty the evolutionary clustering variables. 
*/
template <typename S, typename C>
void CETra<S,C>::freeEvo(){

	staE.clear();

	evoLabels.clear();

	failS.clear();
	ns.clear();
	newS.clear();	
	
}

//------------------------------ Trans. Detection Implementations ------------------------------//

/*
*	Func: 		
*		trackTransitions(search_table&)
*	Args: 
*		The evolutionary clustering search table. 
*	Ret:
*		Transitions object, executes the detection mechanism between currents referential 
*		and evolutionary clustering. 
*/
template <typename S, typename C>
Transitions<C> CETra<S, C>::trackTransitions(search_table& evoTable){

	/* Creating empty transitions object */
	Transitions<C> TRANS;

	/* Inserting statistics names in transitions */
	TRANS.insertInterN(names);

	/* Building overlap matrix between ref and evo clustering and store it in transitions object */
	TRANS.insertExtMatrix(clusterOverlap(evoTable));

	/* Detecting external transitions */
	extTransitions(TRANS);

	/* Detecting internal transitions for surviving clusters */
	if(!TRANS.getSurvs().empty())
		intTransitions(TRANS);

	return TRANS;
}

/*
*	Func: 		
*		clusterOverlap(search_table&)
*	Args: 
*		The evolutionary clustering search table.  
*	Ret:
*		Overlapping matrix, returns the overlapping matrix between currents referential 
*		and evolutionary clustering, and builds fail/new sensors information. 
*/
template <typename S, typename C>
auto CETra<S, C>::clusterOverlap(search_table& evoTable) -> overlaping{
	
	overlaping matrix;

	unordered_map<C,int> lmap = useLabels();

	for (auto &c : clusR){

		/* Each ref cluster has intersection with all the evo clusters */
		matrix[c.first] = vector<float>(evoLabels.size());
		
		for (auto &elem : c.second) {
			
			if(evoTable.find(elem) != evoTable.end()){

				/* Adding the tracked evo sensors weights to the matching index */
				matrix[c.first][lmap[get<0>(evoTable[elem])]]+= get<1>(evoTable[elem]);

				/* Removing tracked evo sensor from the auxiliary set */
				ns.erase(elem);

			}else{

				/* Storing the missing ref sensor alongside it's ref cluster */
				failS[c.first].insert(failS[c.first].end(),elem);
			};
		}
	}

	/* Adding the not tracked evo sensors in the new sensor structure alongside it's evo cluster */
	for(const auto &x: ns){
		newS[get<0>(evoTable[x])].insert(newS[get<0>(evoTable[x])].end(), x);
	}

	//showIntersection(matrix);

	/* Updating the matrix with the overlapping between the intersections and the ref clusters weights */
	for (const auto &i : matrix){
		for (auto x = 0; x < i.second.size(); x++){
			matrix[i.first][x] =  matrix[i.first][x]/refW[i.first];
		}
	}

	//showOverlaping(matrix);

	return matrix;
}

/*
*	Func: 		
*		useLabels()
*	Args: 
*		None. 
*	Ret:
*		Hash Table, maps evolutionary cluster labels into matrix indexes.
*/
template <typename S, typename C>
unordered_map<C,int> CETra<S, C>::useLabels(){

	unordered_map<C, int> lmap;

	int i = 0;
	for(const auto &x: evoLabels){
		lmap[x] = i;
		i++;
	}

	return lmap;
}

//------------------------------- Ext. Transitions Implementations -------------------------------//

/*
*	Func: 		
*		extTransitions(Transitions<C>&)
*	Args: 
*		The overlapping matrix,
*		The transitions object. 
*	Ret:
*		None, detects external transitions between the currents referential and evolutionary
*		clustering.
*/
template <typename S, typename C>
void CETra<S, C>::extTransitions(Transitions<C>& TRANS){

	const overlaping &matrix = TRANS.getExtMatrix();

	unordered_map<int,C> lmap = hashLabels();

	unordered_map<C, vector<C>> scands;
	set<C> tracks;

	for(const auto &X : matrix){

		vector<C> split_cand;

		C split_union = (C) -2;
		C surv_cand = (C) -2;

		for (int Y = 0; Y < X.second.size(); Y++){

			float mcell = X.second[Y];

			if (mcell >= surv_limit){
				
				if(surv_cand = (C) -2){
					surv_cand = lmap[Y];
				
				}else if(mcell > X.second[surv_cand]){

					surv_cand = lmap[Y];
				
				}else if(mcell == X.second[surv_cand]){

					if(clusR[surv_cand].size() < clusR[lmap[Y]].size()){
						surv_cand = lmap[Y];
					}
				}

			}else if(mcell >= split_limit) {
				
				split_cand.insert(split_cand.end(), Y);
			}

		}

		if(surv_cand == (C) -2 && split_cand.empty() == true){

			TRANS.insertDeath(X.first);

			checkFailDeath(X.first, TRANS);

		}else if(split_cand.empty() == false){

			float split_weis = sumSplits(X.second, split_cand);

			if(split_weis >= surv_limit){

				for(const auto x: split_cand) {
					
					TRANS.insertSplits(X.first, lmap[x]);

					tracks.insert(lmap[x]);
				}
			
			}else if(surv_cand != (C) -2){

				scands[surv_cand].insert(scands[surv_cand].end(), X.first);
								
			}else TRANS.insertDeath(X.first);


		}else scands[surv_cand].insert(scands[surv_cand].end(), X.first);

	}

	for(const auto &y: scands){

		if(scands[y.first].size() > 1) TRANS.insertUnion(y.first, y.second);

		else TRANS.insertSurv(scands[y.first][0], y.first);;

		tracks.insert(y.first);

	}

	set_difference(evoLabels.begin(), evoLabels.end(), tracks.begin(), tracks.end(),
        inserter(TRANS.allocBirths(), TRANS.allocBirths().begin()));

	checkNewBirth(TRANS);

}

/*
*	Func: 		
*		hashLabels()
*	Args: 
*		None. 
*	Ret:
*		Hash Table, maps matrix indexes to the evolutionary clusters labels.
*/
template <typename S, typename C>
unordered_map<int,C> CETra<S, C>::hashLabels(){

	unordered_map<int, C> umap;

	int i = 0;
	for(const auto &x: evoLabels){
		umap[i] = x;
		i++;
	}
	return umap;
}

/*
*	Func: 		
*		sumSplits(const vector<float>, const vector<C>)
*	Args: 
*		Array with a referential cluster overlaps,
*		Array with evolutionary clusters labels that are split candidates 
*			to a referential clustering.  
*	Ret:
*		Float, the overlaps sum of a referential clustering split candidates.
*/
template <typename S, typename C>
float CETra<S, C>::sumSplits(const vector<float> &overlaps, const vector<C> &split_cand){

	float sum = 0.0;

	for(const auto &x: split_cand){
		sum+=overlaps[x];
	}

	return sum;
}

/*
*	Func: 		
*		checkFailDeath(const C &, Transitions<C>&)
*	Args: 
*		A dying cluster,
*		The transitions object. 
*	Ret:
*		None, identifies if a cluster is dead because of missing sensors.
*/
template <typename S, typename C>
void CETra<S, C>::checkFailDeath(const C &clu, Transitions<C>& TRANS){

	if(failS.find(clu) != failS.end()){
		if(failS[clu].size()/staR[clu][0] > surv_limit)
			TRANS.insertFDeath(clu);
	}
}

/*
*	Func: 		
*		checkNewBirth(Transitions<C>&);
*	Args: 
*		The transitions object. 
*	Ret:
*		None, identifies if a new cluster emerged because of new sensors.
*/
template <typename S, typename C>
void CETra<S, C>::checkNewBirth(Transitions<C>& TRANS){

	for(const auto &x: TRANS.getBirths()){
		if(newS.find(x) != newS.end()){
			if(newS[x].size()/staE[x][0] > surv_limit)
				TRANS.insertNBirth(x);
		}
	}
}

//------------------------------- Int. Transitions Implementations -------------------------------//

/*
*	Func: 		
*		intTransitions(Transitions<C>&)
*	Args: 
*		The transitions object. 
*	Ret:
*		None, compares the internal statistics of the survived referential clusters 
*		with the corresponding evolutionary cluster and put it in a queue.		
*/
template <typename S, typename C>
void CETra<S, C>::intTransitions(Transitions<C>& TRANS){

	statistics inter;

	for(const auto &x: TRANS.getSurvs()){
		for(int i = 0; i < staR[x.first].size(); i++){
			if(i < staE[x.second].size())
				inter[x.first].insert(inter[x.first].end(), staE[x.second][i]/staR[x.first][i]);
		}
	}

	TRANS.insertIntMatrix(inter);

	if(staQueue.size() < winSize){
		staQueue.push_back(inter);
	}else{
		staQueue.pop_front();
		staQueue.push_back(inter);
	}

	if(staQueue.size() == winSize) 
		checkIntTrans(TRANS);
}

/*
*	Func: 		
*		checkIntTrans(Transitions<C>&)
*	Args: 
*		The transitions object. 
*	Ret:
*		None, checks if the internal statistics changes matches the 
*		entire window size to identify an internal transition.
*/
template <typename S, typename C>
void CETra<S,C>::checkIntTrans(Transitions<C>& TRANS){

	unordered_map<C, vector<int>> counts;

	for(const auto &x: TRANS.getSurvs()){
		counts[x.first] = vector<int>(limits.size());
	} 

	for(const auto &cluS: staQueue){
		for(const auto &clu: cluS){
			for(int i = 0; i < clu.second.size(); i++){
				if(fabs(clu.second[i]-1) > limits[i]){
					counts[clu.first][i]+=1;
				}
			}
		}	
	}

	for(const auto &x: counts){
		int i = 0;
		for(const auto &y: x.second){
			if(y == staQueue.size()) {
				TRANS.insertInterC(x.first, i);
				checkNewStatistic(x.first, TRANS.getSurvs()[x.first], TRANS);
			}
			i++;
		}
	}

}

/*
*	Func: 		
*		checkNewStatistic(const C &, const C &, Transitions<C>&)
*	Args: 
*		A referential cluster,
*		An evolutionary cluster,
*		The transitions object.
*	Ret:
*		None, inserts a size change by many new sensors. 
*/
template <typename S, typename C>
void CETra<S,C>::checkNewStatistic(const C &cr, const C &ce, Transitions<C>& TRANS){

	if(newS[ce].size() > staR[cr][0] * limits[0]){
		TRANS.insertNSurv(cr, ce);
	}

}

//------------------------------- Visualization Implementations -------------------------------//

/*
*	Func: 		
*		showRefClustering()
*	Args: 
*		None. 
*	Ret:
*		None, show referential clustering and it's clusters weights.
*/
template <typename S, typename C>
void CETra<S, C>::showRefClustering(){

	cout << "------Clustering------" << endl;
	for (const auto &i : clusR){
		cout << i.first << ": "; 
		for (const auto &x : i.second) cout << x << " ";
		cout << endl;
	}

	cout << "----Cluster Weights----" << endl;
	for(const auto &x : refW) cout << x.first << ": " << x.second << endl;

}

/*
*	Func: 		
*		showStatistics()
*	Args: 
*		None. 
*	Ret:
*		None, show internal statistics.
*/
template <typename S, typename C>
void CETra<S, C>::showStatistics(){
	
	cout << "------Statistics-------" << endl;

	for(const auto &x: staR){
		cout << x.first << ": ";
		for(const auto &y: x.second){
			cout << "( " << y << " )";
		}
		cout << endl;
	}

	cout << endl;

	for(const auto &x: staE){
		cout << x.first << ": ";
		for(const auto &y: x.second){
			cout << "( " << y << " )";
		}
		cout << endl;
	}

	cout << endl;

}

/*
*	Func: 		
*		showIntersection(const overlaping&)
*	Args: 
*		The overlapping matrix. 
*	Ret:
*		None, show intersections.
*/
template <typename S, typename C>
void CETra<S,C>::showIntersection(const overlaping& matrix){

	cout << "-----Intersection-----" << endl;
	for (const auto &i : matrix){
		cout << i.first << ": "; 
		for (const auto &x : i.second) cout << x << " ";
		cout << endl;
	}
	cout << endl;
}

/*
*	Func: 		
*		showOverlaping(const overlaping&)
*	Args: 
*		The overlapping matrix. 
*	Ret:
*		None, show overlapping.
*/
template <typename S, typename C>
void CETra<S,C>::showOverlaping(const overlaping& matrix){

	cout << "-----Overlapping-----" << endl;
	for (const auto &i : matrix){
		cout << i.first << ": "; 
		for (const auto &x : i.second) cout << x << " ";
		cout << endl;
	}
	cout << endl;
}

/*
*	Func: 		
*		showTrackStatistics()
*	Args: 
*		None. 
*	Ret:
*		None, show failing and new sensors.
*/
template <typename S, typename C>
void CETra<S,C>::showTrackStatistics(){

	if(!failS.empty()){

		cout << "Faulty sensors" << endl;
		for(const auto &x: failS){
			cout << x.first << ": ";
			for(const auto &y: x.second) cout << y << " ";
			cout << endl;
		}
		cout << endl;
	}
	
	if(!newS.empty()){
		cout << "New sensors" << endl;
		for(const auto &x: newS){
			cout << x.first << ": ";
			for(const auto &y: x.second) cout << y << " ";
			cout << endl;
		}
		cout << endl;
	}
}

/*
*	Func: 		
*		showIntQueue()
*	Args: 
*		None. 
*	Ret:
*		None, show statistics inside the queue.
*/
template <typename S, typename C>
void CETra<S,C>::showIntQueue(){

	for(const auto &x: staQueue){
		cout << "----------" << endl;
		for(const auto &z: x){
			cout << z.first << ": ";
			for(const auto &y: z.second){
				cout << y << " ";
			}
			cout << endl;
		}
	}
}

#endif