/* Author: Afonso Matheus                                                     */
/* Date: 2021                                                                 */
/* Ver. Beta                                                                  */
//----------------------------------------------------------------------------//
//                                                                            //                                                                          //
// Script that contains the implementation of cluster's transiction detection // 
// between clusterings made by both conventional and data streams algorithms. //
//                                                                            //
//----------------------------------------------------------------------------//

#ifndef TRACES_H
#define TRACES_H

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
class TraCES{

	/* Nicknames for code simplification */

	using clustering = unordered_map<C, vector<S> >;
	using overlaping = unordered_map<C, vector<float> >;
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

		/* Referencial Clustering variables */

		clustering clusR;
		unordered_map<C,float> refW;
		statistics staR;
		int starSize = 0;

		/* Evolutional Clustering variables */

		//clustering clusE;
		//unordered_map<C,float> evoW;
		statistics staE;
		set<C> evoLabels;

		/* Tracking variables */

		search_table evoTable;
		overlaping matrix;

		/* Fail/New sensors variables */

		unordered_map<C, vector<S>> failS;
		unordered_set<S> ns;
		unordered_map<C, vector<S>> newS;

		/* Transitions variables */
		
		Transitions<C> TRANS;
		deque<statistics> staQueue;

		/**************************** SIGNATURES ****************************/

		//---------- Ref. Clustering Methods ----------//

		void storeClustering(const vector<S>&, const vector<C>&, const vector<float>&);

		template <class T>
		void storeRefStatistics(initializer_list<T>);

		void freeRef();

		//---------- Evo. Clustering Methods ----------//

		void getEvoInformation(const vector<S>&, const vector<C>&, const vector<float>&);

		template <class T>
		void storeEvoStatistics(initializer_list<T>);

		void freeEvo();

		//--------- Trans. Detection Methods ---------//

		void trackTransitions();

		void clusterOverlap();
		
		unordered_map<C,int> useLabels();

		//------------- Ext. Transitions -------------//

		void extTransitions();

		float sumSplits(const vector<float>&, const vector<C>&);

		unordered_map<int,C> hashLabels();

		void checkFailDeath(const C &);

		void checkNewBirth();

		//------------- Int. Transitions -------------//
		
		void intTransitions();

		void checkIntTrans();

		void checkNewStatistic(const C &, const C &);

		//--------------- Visualization ---------------//

		void showRefClustering();

		void showStatistics();

		void showIntersection();

		void showOverlaping();

		void showTrackStatistics();

		void showTransitions();

		void showIntQueue();

	public:

		TraCES(const int = 3, const vector<float> = {});

		//----------- Execution -----------//

		void execute(const vector<S>&, const vector<C> &, const vector<float> &);

		template <class T>
		void execute(const vector<S>&, const vector<C> &, const vector<float> &, initializer_list<T>);

		//---------- User Configs ----------//

		void configSizeLimit(const float&);

		void configStaNames(const vector<string>&);

		void configSurvLimit(const float&);

		void configSplitLimit(const float&);
	
};


/************************** IMPLEMENTATIONS **************************/

/*
*
*	Func: 		
*		TraCES(const int, const vector<float>) *Constructor
*	Args: 
*		Size of statistics window, 
*		Limits of the internal statistics.
*	Ret:
*		None, constructs a TraCES object. 
*
*/
template <typename S, typename C>
TraCES<S, C>::TraCES(const int N, const vector<float> lim){

	winSize = N;

	limits = lim;
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
*		None, executes the transiction detection alghoritm with only the size 
*		internal statistic. 
*
*/
template <typename S, typename C>
void TraCES<S, C>::execute(const vector<S> &sen, const vector<C> &clu, const vector<float> &wei){
	
	try{

		/* The method must not be executed if the sensors/clusters/weights arrays don't have the same sizes */

		if(sen.size() != clu.size() or sen.size() != wei.size()){
			throw invalid_argument("The sensors/clusters/weights arrays are not the same size!");
		}

		/* Command for first execution, when there is no referencial clustering */

		if(clusR.empty()){

			/* Storing the current cluestering and it's clusters weights */

			storeClustering(sen, clu, wei);

			/* Storing the clusters sizes and the total tracked internal statistics*/

			for (const auto &x: clusR) staR[x.first].insert(staR[x.first].end(), x.second.size());
			starSize = 1;

			showRefClustering();

		}else{

			cout << endl << "///// NEXT WINDOW /////" << endl << endl;

			/* Getting all necessary evolutionary clustering variables, including the evolved clusters sizes */

			getEvoInformation(sen, clu, wei);

			/* Monitoring the trasintions between the current referencial clustering and the current evolutionary clustering */

			trackTransitions();

			/* Showing survived clusters, if any */

			if(!TRANS.getSurvs().empty()) TRANS.showSurvs();

			/* Verifing if there was any detected internal or external transitions */

			if(TRANS.checkExtChange() || TRANS.checkIntChange()){

				cout << "=== TRANS DETECTED ===" << endl << endl;

				/* Showing the detected internal or/and external transitions */

				showTransitions();

				cout << "### UPD REF CLUSTERING ###" << endl << endl;

				/* Freeing the referencial clustering variables */

				freeRef();

				/* Updating the referencial clustering variables with the current evolutionary clustering */

				storeClustering(sen, clu, wei);

				/*clusR = clusE;
				refW = evoW;*/
				staR = staE;

				showRefClustering();

			}

			/* Freeing the evolutionary clustering variables */

			freeEvo();
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
*		List with the desired internal clusters statistiscs.
*	Ret:
*		None, executes the transiction detection alghoritm with the desired
*		internal clusters statistiscs.  
*
*/
template <typename S, typename C>
template <class T>
void TraCES<S,C>::execute(const vector<S> &sen, const vector<C> &clu, const vector<float> &wei, initializer_list<T> list){

	try{

		/* The method must not be executed if the sensors/clusters/weights arrays don't have the same sizes */

		if(sen.size() != clu.size() or sen.size() != wei.size()){
			throw invalid_argument("The sensors/clusters/weights arrays are not the same size!");
		}

		/* The method must not be executed if the internal statistics size changed since last execution! */

		if(starSize > 0 && starSize != list.size()){
			throw invalid_argument("The internal statistics size changed since last execution!");
		}

		/* The method must not be executed if the inputed limits are more numerous than the inputed statistics */

		if (list.size() + 1 < limits.size()){
			throw invalid_argument("The limits array size is greater than the input statistics list!");
		}

		/* The method must not be executed if the inputed statistics names are more numerous than the inputed statistics */
		
		if(names.size() > list.size() + 1){
			throw invalid_argument("The amount of statistics names surpasses the statistics list!");
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

		/* Command for first execution, when there is no referencial clustering */

		if(clusR.empty()){

			/* Storing the current cluestering and it's clusters weights */

			storeClustering(sen, clu, wei);

			/* Storing the clusters sizes plus the internal statistics inputed */
			
			storeRefStatistics(list);

			showRefClustering();

		}else{

			cout << endl << "/////////////// NEXT WINDOW ///////////////" << endl << endl;

			/* Getting all necessary evolutionary clustering variables, including the evolved clusters sizes */

			getEvoInformation(sen, clu, wei);

			/* Storing the internal statistics inputed */
			
			storeEvoStatistics(list);

			/* Monitoring the trasintions between the current referencial clustering and the current evolutionary clustering */

			trackTransitions();

			/* Showing survived clusters, if any */

			if(!TRANS.getSurvs().empty()) TRANS.showSurvs();

			/* Verifing if there was any detected internal or external transitions */

			if(TRANS.checkExtChange() || TRANS.checkIntChange()){

				cout << "=== TRANS DETECTED ===" << endl << endl;

				/* Showing the detected internal or/and external transitions */

				showTransitions();

				cout << "### UPD REF CLUSTERING ###" << endl << endl;

				/* Freeing the referencial clustering variables */

				freeRef();

				/* Updating the referencial clustering variables with the current evolutionary clustering */

				storeClustering(sen, clu, wei);
				//clusR = clusE;
				//refW = evoW;
				staR = staE;

				showRefClustering();

			}

			/* Freeing the evolutionary clustering variables */

			freeEvo();
		}

	}catch(invalid_argument& e){

		cerr << e.what() << endl;

		throw -1;

	}catch(length_error& e){

		cerr << e.what() << endl;

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
void TraCES<S,C>::configSizeLimit(const float& sl){
	limits[0] = sl;
}

/*
*	Func: 		
*		configStaNames(const vector<string>&)
*	Args: 
*		Array with the user defined arrays name. 
*	Ret:
*		None, updates the names in the internal statistics names array.  
*/
template <typename S, typename C>
void TraCES<S,C>::configStaNames(const vector<string>& nam){
	for(const auto &x: nam) 
		names.insert(names.end(), x);
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
void TraCES<S,C>::configSurvLimit(const float& ls){
	if(ls < split_limit){
		cerr << "The survival limit must be higher than the current split limit" << endl;
		return;
	}

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
void TraCES<S,C>::configSplitLimit(const float& ls){
	
	if(ls >= surv_limit){
		cerr << "The split limit must be lower than the current survival limit" << endl;
		return;
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
*		None, stores the referencial clustering and it's clusters weights in an internal
*		hash table each. 
*/
template <typename S, typename C>
void TraCES<S, C>::storeClustering(const vector<S> &sen, const vector<C> &clu, const vector<float> &wei){

	/* Storing referencial clustering clusters and it's clusters weights */
	for (int i = 0; i < sen.size(); ++i){
		clusR[clu[i]].insert(clusR[clu[i]].end(), sen[i]);
		refW[clu[i]] += wei[i];
	}
}


/*
*	Func: 		
*		storeRefStatistics(initializer_list<T>)
*	Args: 
*		List with all the internal statistics. 
*	Ret:
*		None, stores the inputed referencial clusters internal statistics in a hash table. 
*/
template <typename S, typename C>
template <class T>
void TraCES<S,C>::storeRefStatistics(initializer_list<T> list){

	/* The set structure helps to detect the clustering labels maintaning ordering */

	set<C> ref_labels;

	/* Storing referencial clustering clusters's sizes */

	for (const auto &x: clusR){
		if(ref_labels.find(x.first) == ref_labels.end()) ref_labels.insert(x.first);
		staR[x.first].insert(staR[x.first].end(), x.second.size()); 
	}

	/* Storing referencial clustering clusters's internal statistics */

	for(auto &s: list){
		auto itr = ref_labels.begin();

		/* Abort if the detected labels size are different from the inputed statistics size */

		if(s.size() != ref_labels.size()){
			throw invalid_argument("The labels/statistics arrays are not the same size!");
		}

		for(const auto &x: s){
			staR[*itr].insert(staR[*itr].end(), x);
			itr++;
		}		 
	}

	/* Storing internal statistics size for future verification */

	starSize = list.size();		

}

/*
*	Func: 		
*		freeRef()
*	Args: 
*		None 
*	Ret:
*		None, empty the referencial clusterings variabels. 
*/
template <typename S, typename C>
void TraCES<S,C>::freeRef(){

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
*		None, build all the necessary evolutionary information, such as the evolutionary labels,
*		the search table and the clusters size internal statistics. 
*/
template <typename S, typename C>
void TraCES<S,C>::getEvoInformation(const vector<S> &sen, const vector<C> &clu, const vector<float> &wei){

	for (int i = 0; i < sen.size(); ++i){

		//clusE[clu[i]].insert(clusE[clu[i]].end(), sen[i]);
		//evoW[clu[i]] += wei[i];

		/* Getting evolutionay clustering cluster's labels */

		if(evoLabels.find(clu[i]) == evoLabels.end()) 
			evoLabels.insert(clu[i]);

		/* Building evolutionay clustering search table */

		evoTable[sen[i]] = make_tuple(clu[i],wei[i]);

		/* Building evolutionay clustering cluster's sizes */

		if(staE.find(clu[i]) == staE.end())
			staE[clu[i]].insert(staE[clu[i]].end(), 0);
		staE[clu[i]][0] += 1;

		/* Copying evolutionay clustering sensors in an auxiliar structure for new sensor detection */

		ns.insert(sen[i]);
	}

}


/*
*	Func: 		
*		storeEvoStatistics(initializer_list<T>)
*	Args: 
*		List with all the internal statistics.
*	Ret:
*		None, stores the inputed evolutionary clusters internal statistics in a hash table. 
*/
template <typename S, typename C>
template <class T>
void TraCES<S,C>::storeEvoStatistics(initializer_list<T> list){

	/* Storing evolutionary clustering clusters's internal statistics */

	for(const auto &s: list){
		auto itr = evoLabels.begin();

		/* Abort if the previously detected evo labels size are different from the inputed statistics size */

		if(s.size() != evoLabels.size()){
			throw invalid_argument("The labels/statistics arrays are not the same size!");
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
*		None, empty the evolutionary clusterings variabels. 
*/
template <typename S, typename C>
void TraCES<S,C>::freeEvo(){

	//clusE.clear();
	//evoW.clear();
	staE.clear();
	
	evoLabels.clear();
	evoTable.clear();
	matrix.clear();

	failS.clear();
	ns.clear();
	newS.clear();

	TRANS.clear();	
	
}

//------------------------------ Trans. Detection Implementations ------------------------------//

/*
*	Func: 		
*		trackTransitions()
*	Args: 
*		None. 
*	Ret:
*		None, executes the detection mechanism between currents referencial 
*		and evolutionary clusterings. 
*/
template <typename S, typename C>
void TraCES<S, C>::trackTransitions(){

	/* Building overlap matrix between ref and evo clusterings */

	clusterOverlap();

	//showTrackStatistics();

	/* Detecting external transitions */

	extTransitions();

	//showTrackStatistics();

	/* Detecting internal transitions for surviving clusters */

	if(!TRANS.getSurvs().empty())
		intTransitions();
}

/*
*	Func: 		
*		clusterOverlap()
*	Args: 
*		None. 
*	Ret:
*		None, builds the overlapping matrix between currents referencial 
*		and evolutionary clusterings, and builds fail/new sensors information. 
*/
template <typename S, typename C>
void TraCES<S, C>::clusterOverlap(){
	
	unordered_map<C,int> lmap = useLabels();

	for (auto &c : clusR){

		/* Each ref cluster has intersection with all the evo clusters */
		
		matrix[c.first] = vector<float>(evoLabels.size());
		
		for (auto &elem : c.second) {
			
			if(evoTable.find(elem) != evoTable.end()){

				/* Adding the tracked evo sensors weights to the matching index */
				
				matrix[c.first][lmap[get<0>(evoTable[elem])]]+= get<1>(evoTable[elem]);

				/* Removing tracked evo sensor from the auxiliar set */

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

	//showIntersection();

	/* Updating the matrix with the overlapping between the intersections and the ref clusters weights */

	for (const auto &i : matrix){
		for (auto x = 0; x < i.second.size(); x++){
			matrix[i.first][x] =  matrix[i.first][x]/refW[i.first];
		}
	}

	showOverlaping();
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
unordered_map<C,int> TraCES<S, C>::useLabels(){

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
*		extTransitions()
*	Args: 
*		None. 
*	Ret:
*		None, detects external transitions between the currents referencial and evolutionary
*		clusterings.
*/
template <typename S, typename C>
void TraCES<S, C>::extTransitions(){

	unordered_map<int,C> lmap = hashLabels();

	unordered_map<C, vector<C>> scands;
	set<C> tracks;

	for(const auto &X : matrix){

		vector<C> split_cand;

		C split_union = (C) -2;
		C surv_cand = (C) -2;

		for (int Y = 0; Y < X.second.size(); Y++){

			float mcell = X.second[Y];

			if (mcell > surv_limit){
				
				if(surv_cand = (C) -2){
					surv_cand = lmap[Y];
				
				}else if(mcell > X.second[surv_cand]){

					surv_cand = lmap[Y];
				
				}else if(mcell == X.second[surv_cand]){

					if(clusR[surv_cand].size() < clusR[lmap[Y]].size()){
						surv_cand = lmap[Y];
					}
				}

			}else if(mcell > split_limit) {
				
				split_cand.insert(split_cand.end(), Y);
			}

		}

		if(surv_cand == (C) -2 && split_cand.empty() == true){

			TRANS.insertDeath(X.first);

			checkFailDeath(X.first);

		}else if(split_cand.empty() == false){

			float split_weis = sumSplits(X.second, split_cand);

			if(split_weis > surv_limit && split_weis > X.second[surv_cand]){

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

		else TRANS.insertSurv(make_tuple(scands[y.first][0], y.first));

		tracks.insert(y.first);

	}

	set_difference(evoLabels.begin(), evoLabels.end(), tracks.begin(), tracks.end(),
        inserter(TRANS.allocBirths(), TRANS.allocBirths().begin()));

	checkNewBirth();

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
unordered_map<int,C> TraCES<S, C>::hashLabels(){

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
*		Array with a referencial cluster overlaps,
*		Array with evolutionary clusters labels that are split candidates 
*			to a referencial clustering.  
*	Ret:
*		Float, the overlaps sum of a referencial clustering split candidates.
*/
template <typename S, typename C>
float TraCES<S, C>::sumSplits(const vector<float> &overlaps, const vector<C> &split_cand){

	float sum = 0;

	for(const auto &x: split_cand){
		sum+=overlaps[x];
	}

	return sum;
}


/*
*	Func: 		
*		checkFailDeath(const C &)
*	Args: 
*		A dying cluster. 
*	Ret:
*		None, identifies if a cluster is dead because of missing sensors.
*/
template <typename S, typename C>
void TraCES<S, C>::checkFailDeath(const C &clu){

	if(failS.find(clu) != failS.end()){
		if(failS[clu].size()/staR[clu][0] > surv_limit)
			TRANS.insertFDeath(clu);
		
	}
}

/*
*	Func: 		
*		checkNewBirth();
*	Args: 
*		None. 
*	Ret:
*		None, identifies if a new cluster emerged because of new sensors.
*/
template <typename S, typename C>
void TraCES<S, C>::checkNewBirth(){

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
*		intTransitions()
*	Args: 
*		None. 
*	Ret:
*		None, compares the internal statistics of the survived referencial clusters 
*		with the corresponding evolutionary cluster and put it in a queue.		
*/
template <typename S, typename C>
void TraCES<S, C>::intTransitions(){

	statistics inter;

	for(const auto &x: TRANS.getSurvs()){
		for(int i = 0; i < staR[get<0>(x)].size(); i++){
			inter[get<0>(x)].insert(inter[get<0>(x)].end(), staE[get<1>(x)][i]/staR[get<0>(x)][i]);
		}
	}

	if(staQueue.size() < winSize){
		staQueue.push_back(inter);
	}else{
		staQueue.pop_front();
		staQueue.push_back(inter);
	}

	if(staQueue.size() == winSize) 
		checkIntTrans();
}

/*
*	Func: 		
*		checkIntTrans()
*	Args: 
*		None. 
*	Ret:
*		None, checks if the internal statistics changes matches the 
*		entire window size to identify an internal transiction.
*/
template <typename S, typename C>
void TraCES<S,C>::checkIntTrans(){

	unordered_map<C, vector<int>> counts;
	unordered_map<C, C> saux;

	for(const auto &x: TRANS.getSurvs()){
		counts[get<0>(x)] = vector<int>(limits.size());
		saux[get<0>(x)] = get<1>(x);
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
				checkNewStatistic(x.first, saux[x.first]);
			}
			i++;
		}
	}

}

template <typename S, typename C>
void TraCES<S,C>::checkNewStatistic(const C &cr, const C &ce){

	if(newS[ce].size() > staR[cr][0] * limits[0]){
		TRANS.insertNSurv(make_tuple(cr,ce));
	}

}

//------------------------------- Visualization Implementations -------------------------------//

/*
*	Func: 		
*		showRefClustering()
*	Args: 
*		None. 
*	Ret:
*		None, show referencial clustering and it's clusters weights.
*/
template <typename S, typename C>
void TraCES<S, C>::showRefClustering(){

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
void TraCES<S, C>::showStatistics(){
	
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
*		showIntersection()
*	Args: 
*		None. 
*	Ret:
*		None, show intersections.
*/
template <typename S, typename C>
void TraCES<S,C>::showIntersection(){

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
*		showOverlaping()
*	Args: 
*		None. 
*	Ret:
*		None, show overlappings.
*/
template <typename S, typename C>
void TraCES<S,C>::showOverlaping(){

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
void TraCES<S,C>::showTrackStatistics(){

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
*		showTransitions()
*	Args: 
*		None. 
*	Ret:
*		None, show external and internal transitions, if any.
*/
template <typename S, typename C>
void TraCES<S, C>::showTransitions(){

	if(TRANS.checkExtChange()){

		cout << "+ EXT TRANSITIONS" << endl << endl; 

		if(!TRANS.getSplits().empty()) TRANS.showSplits();

		if(!TRANS.getUnions().empty()) TRANS.showUnions();

		if(!TRANS.getDeaths().empty()){ 
			TRANS.showDeaths();
			TRANS.showFDeaths();
		}

		if(!TRANS.getBirths().empty()){
			TRANS.showBirths();
			TRANS.showNBirths();
		}

	}

	if(TRANS.checkIntChange()){

		cout << "+ INT TRANSITIONS" << endl << endl;

		if(!TRANS.getInterC().empty()) {
			TRANS.showInterC(names);
			TRANS.showNStatistic();
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
void TraCES<S,C>::showIntQueue(){

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