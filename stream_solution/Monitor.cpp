#include "Monitor.h"

template <typename S, typename C>
void Monitor<S, C>::execute(const vector<S> &sen, const vector<C> &clu, const vector<float> &wei, const vector<C> &lab){
	
	try{

		if(sen.size() != clu.size() or sen.size() != wei.size()){
			throw invalid_argument("The sensors/clusters/weights arrays are not the same size!");
		}

		if(clusR.empty()){

			storeClustering(sen, clu, wei);

		}else{

			storeEvoLabels(lab);

			setSizes(sen, clu);

			checkEvolution(sen, clu, wei);

			if(!TRANS.getSurvs().empty()) showSurvs();

			if(TRANS.checkExtChange() || TRANS.checkIntChange()){

				showTransitions();

				freeRef();

				storeClustering(sen, clu, wei);

				staR = staE;

			}

			freeEvo();
		}

	}catch(invalid_argument& e){

		cerr << e.what() << endl;

	}

}

template <typename S, typename C>
void Monitor<S,C>::configSizeLimit(const float& sl){
	limits[0] = sl;}

template <typename S, typename C>
void Monitor<S,C>::configStaNames(const vector<string>& nam){
	for(const auto &x: nam) names.insert(names.end(), x);}

template <typename S, typename C>
void Monitor<S,C>::configNewLimit(const float& ln){
	new_limit = ln;}

template <typename S, typename C>
void Monitor<S,C>::configFailLimit(const float& lf){
	fail_limit = lf;}

/////////////////////////////////////////////////////////////

template <typename S, typename C>
void Monitor<S, C>::storeClustering(const vector<S> &sen, const vector<C> &clu, const vector<float> &wei){

	sizeR = sen.size();

	//pensar no tamanho dos vetores para cada cluster
	for (int i = 0; i < sen.size(); ++i){
		if(clu[i] != -1) clusR[clu[i]].insert(clusR[clu[i]].end(), sen[i]); 
	}
	
	cout << "------Clustering------" << endl;
	for (const auto &i : clusR){
		cout << i.first << ": "; 
		for (const auto &x : i.second) cout << x << " ";
		cout << endl;
	}

	clusterWeights(sen, wei);

}

template <typename S, typename C >
void Monitor<S, C>::clusterWeights(const vector<S> &sen, const vector<float> &wei) {

	unordered_map<S,float> sw = senWei(sen, wei);

	for (const auto &i : clusR){
		for(auto x : i.second) cluW[i.first]+= sw[x];
	}
	
	cout << "----Cluster Weights----" << endl;
	for(const auto &x : cluW) cout << x.first << ": " << x.second << endl;


}

template <typename S, typename C >
unordered_map<S, float> Monitor<S, C>::senWei(const vector<S> &sen, const vector<float> &wei){

	unordered_map<S,float> umap(wei.size());

	for (int i = 0; i < sen.size(); ++i)
		umap.insert({sen[i], wei[i]});

	return umap;

}



template <typename S, typename C>
void Monitor<S, C>::showStatistics(){
	
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

template <typename S, typename C>
void Monitor<S, C>::setSizes(const vector<S> &sen, const vector<C> &clu){

	int fails = 0;

	if(sizeR > sen.size()){
		throw invalid_argument("The current size is shorter than the size of the ref clustering");
	}else{
		TRANS.setNewRatio((float) (sen.size() - sizeR)/sizeR);
	}

	for (auto &x: labels){
	 	staE[x].insert(staE[x].end(), 0); 
	}

	for (int i = 0; i < sizeR; ++i){
		if(clu[i] != -1){
			staE[clu[i]][0] += 1;
		}else{
			fails++;
		} 
	}

	TRANS.setFailRatio((float) fails/sizeR);

	if(TRANS.getFailRatio() > fail_limit){
		throw domain_error("Too many failing samples!");
	}

}

/////////////////////////////////////////////////////////////

template <typename S, typename C>
void Monitor<S, C>::checkEvolution(const vector<S>& sen, const vector<C>& clu, const vector<float>& wei){

	clusterOverlap(sen, clu, wei);

	extTransitions();

	intTransitions();
}

template <typename S, typename C>
auto Monitor<S,C>::makeSearchTable(const vector<S> &sen, const vector<C> &clu, const vector<float> &wei) -> search_table{

	search_table clusE; 

	for (int i = 0; i < sizeR; ++i)
		clusE.insert({sen[i], make_tuple(clu[i],wei[i])});

	return clusE;

}

template <typename S, typename C>
void Monitor<S, C>::storeEvoLabels(const vector<C> &lab){

	if(labels.empty()){
		labels = lab;
	}else if(labels != lab && labels.size() == lab.size()){
		throw invalid_argument("Labels changed after the first evolution!");
	}

	//sort(labels.begin(),labels.end());

}

/////////////////////////////////////////////////////////////


template <typename S, typename C>
void Monitor<S, C>::clusterOverlap(const vector<S> &sen, const vector<C> &clu, const vector<float> &wei){

	search_table clusE = makeSearchTable(sen, clu, wei);

	unordered_map<C,int> lmap = useLabels();
	
	for (auto &c : clusR){
		matrix[c.first] = vector<float>(labels.size()); //saber quantidade de grupos do agrupamento 2
		for (auto &elem : c.second) {
			if(get<0>(clusE[elem]) != -1) matrix[c.first][lmap[get<0>(clusE[elem])]]+= get<1>(clusE[elem]);
		}

	}

	/*cout << "-----Intersection-----" << endl;
	for (const auto &i : matrix){
		cout << i.first << ": "; 
		for (const auto &x : i.second) cout << x << " ";
		cout << endl;
	}
	cout << endl;*/

	for (const auto &i : matrix){
		for (auto x = 0; x < i.second.size(); x++){
			matrix[i.first][x] =  matrix[i.first][x]/cluW[i.first];
		}
	}

	/*cout << "-----Overlapping-----" << endl;
	for (const auto &i : matrix){
		cout << i.first << ": "; 
		for (const auto &x : i.second) cout << x*100 << " ";
		cout << endl;
	}
	cout << endl;*/

}

template <typename S, typename C>
unordered_map<C,int> Monitor<S, C>::useLabels(){

	unordered_map<C, int> lmap;

	int i = 0;
	for(const auto &x: labels){
		lmap[x] = i;
		i++;
	}

	return lmap;

}

/////////////////////////////////////////////////////////////

template <typename S, typename C>
void Monitor<S, C>::extTransitions(){

	unordered_map<int,C> lmap = hashLabels(labels);

	unordered_map<C, vector<C>> scands;
	set<C> tracks;

	for(const auto &X : matrix){

		vector<C> split_cand;

		C split_union = -2;
		C surv_cand = -2;

		for (int Y = 0; Y < X.second.size(); Y++){

			float mcell = X.second[Y];

			if (mcell > 0.5){
				
				if(surv_cand = -2){
					surv_cand = lmap[Y];
				
				}else if(mcell > X.second[surv_cand]){

					surv_cand = lmap[Y];
				
				}else if(mcell == X.second[surv_cand]){

					if(clusR[surv_cand].size() < clusR[lmap[Y]].size()){
						surv_cand = lmap[Y];
					}

				}

			}else if(mcell >= 0.25) split_cand.insert(split_cand.end(), Y);

		}

		if(surv_cand == -2 && split_cand.empty() == true){

			TRANS.insertDeath(X.first);

		}else if(split_cand.empty() == false){

			if(sumSplits(X.second, split_cand) >= 0.5){

				for(const auto x: split_cand) {
					
					TRANS.insertSplits(X.first, lmap[x]);

					tracks.insert(lmap[x]);
				};
			
			}else if(surv_cand != -2){

				scands[surv_cand].insert(scands[surv_cand].end(), X.first);
								
			}else TRANS.insertDeath(X.first);


		}else scands[surv_cand].insert(scands[surv_cand].end(), X.first);

	}

	for(const auto &y: scands){

		if(scands[y.first].size() > 1) TRANS.insertUnion(y.first, y.second);

		else TRANS.insertSurv(make_tuple(scands[y.first][0], y.first));

		tracks.insert(y.first);

	}

	set_difference(labels.begin(), labels.end(), tracks.begin(), tracks.end(),
        inserter(TRANS.allocBirths(), TRANS.allocBirths().begin()));
	
}

template <typename S, typename C>
unordered_map<int,C> Monitor<S, C>::hashLabels(const vector<C> &labels){

	unordered_map<int, C> umap;

	int i = 0;
	for(const auto &x: labels){
		umap[i] = x;
		i++;
	}
	return umap;

}

template <typename S, typename C>
float Monitor<S, C>::sumSplits(const vector<float> &overlaps, const vector<C> &split_cand){

	float sum = 0;

	for(const auto &x: split_cand){
		sum+=overlaps[x];
	}

	return sum;
}

template <typename S, typename C>
void Monitor<S, C>::showTransitions(){

	if(TRANS.checkExtChange()){

		cout << "+ EXT TRANSITIONS" << endl << endl; 

		if(!TRANS.getSplits().empty()) showSplits();

		if(!TRANS.getUnions().empty()) showUnions();

		if(!TRANS.getDeaths().empty()) showDeaths();

		if(!TRANS.getBirths().empty()) showBirths();

	}

	if(TRANS.checkIntChange()){

		cout << "+ INT TRANSITIONS" << endl << endl;

		if(!TRANS.getInterC().empty()) showInterC(names);

		cout << endl;

	}


	
}

/////////////////////////////////////////////////////////////

template <typename S, typename C>
void Monitor<S, C>::intTransitions(){

	statistics inter;
	statistics exter;

	for(const auto &x: TRANS.getSurvs()){

		exter[get<0>(x)] = matrix[get<0>(x)];

		for(int i = 0; i < staR[get<0>(x)].size(); i++){
			inter[get<0>(x)].insert(inter[get<0>(x)].end(), staE[get<1>(x)][i]/staR[get<0>(x)][i]);
		}
	}

	if(qSurvs.size() < winSize){
		qSurvs.push_back(inter);
		qTrans.push_back(exter);

	}else{
		
		qSurvs.pop_front();
		qTrans.pop_front();

		qSurvs.push_back(inter);
		qTrans.push_back(exter);
	}

	//seeIntQueue();
	//seeExQueue();

	if(qSurvs.size() == winSize) checkIntTrans();
	
}

template <typename S, typename C>
void Monitor<S,C>::checkIntTrans(){

	if(qSurvs.size() < winSize) return;

	unordered_map<C, vector<int>> counts;

	for(const auto &x: clusR){
		for(int i = 0; i < limits.size(); i++) counts[x.first].insert(counts[x.first].end(), 0);
	} 

	for(const auto &cluS: qSurvs){
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
			if(y == qSurvs.size()) {
				TRANS.insertInterC(x.first, i);
			}
			i++;
		}
	}

}

template <typename S, typename C>
void Monitor<S,C>::seeIntQueue(){

	for(const auto &x: qSurvs){
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

template <typename S, typename C>
void Monitor<S,C>::seeExQueue(){

	for(const auto &x: qTrans){
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

template <typename S, typename C>
void Monitor<S,C>::freeEvo(){

	staE.clear();
	TRANS.clear();
	matrix.clear();

}

template <typename S, typename C>
void Monitor<S,C>::freeRef(){

	sizeR = 0;
	clusR.clear();
	cluW.clear();
	staR.clear();
	labels.clear();
	qSurvs = {};

}



/////////////////////////////////////////////////////////////


template <typename S, typename C>
void Monitor<S, C>::showSurvs(){TRANS.showSurvs();}

template <typename S, typename C>
void Monitor<S, C>::showSplits(){TRANS.showSplits();}

template <typename S, typename C>
void Monitor<S, C>::showUnions(){TRANS.showUnions();}

template <typename S, typename C>
void Monitor<S, C>::showDeaths(){TRANS.showDeaths();}

template <typename S, typename C>
void Monitor<S, C>::showBirths(){TRANS.showBirths();}

template <typename S, typename C>
void Monitor<S, C>::showInterC(const vector<string> nam){TRANS.showInterC(nam);}


template class Monitor<int, int>;
template class Monitor<char, int>;
template class Monitor<int, char>;
template class Monitor<char, char>;