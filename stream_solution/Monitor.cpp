#include "Monitor.h"

template <typename S, typename C, typename W>
void Monitor<S, C, W>::execute(const vector<S> &sen, const vector<C> &clu, const vector<W> &wei, const vector<C> &lab){
			
	if(clusR.empty()){

		storeClustering(sen, clu, wei, lab);

	}else{

		checkEvolution(sen, clu, wei, lab);

		if(TRANS.checkExt()){

			clusR.clear();
			cluW.clear();

			storeClustering(sen, clu, wei, lab);

		}

		clusE.clear();
		labels.clear();
	}

}

template <typename S, typename C, typename W>
template <class T>
void Monitor<S, C, W>::execute(const vector<S> &sen, const vector<C> &clu, const vector<W> &wei, const vector<C> &lab, initializer_list<T> list){

	if(clusR.empty()){

		storeClustering(sen, clu, wei, lab);

		buildStatistics(lab ,list);

	}else{

		buildStatistics(lab ,list);

		checkEvolution(sen, clu, wei, lab);

		if(TRANS.checkExt()){

			clusR.clear();
			cluW.clear();
			staR.clear();

			storeClustering(sen, clu, wei, lab);
			buildStatistics(lab ,list);

		}

		clusE.clear();
		staE.clear();
		labels.clear();
		//TRANS.clear();

	}

}

/////////////////////////////////////////////////////////////

template <typename S, typename C, typename W>
void Monitor<S, C, W>::storeClustering(const vector<S> &sen, const vector<C> &clu, const vector<W> &wei, const vector<C> &labels){

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

template <typename S, typename C, typename W >
void Monitor<S, C, W>::clusterWeights(const vector<S> &sen, const vector<W> &wei) {

	unordered_map<S,W> sw = senWei(sen, wei);

	for (const auto &i : clusR){
		for(auto x : i.second) cluW[i.first]+= sw[x];
	}
	
	cout << "----Cluster Weights----" << endl;
	for(const auto &x : cluW) cout << x.first << ": " << x.second << endl;


}

template <typename S, typename C, typename W >
unordered_map<S, W> Monitor<S, C, W>::senWei(const vector<S> &sen, const vector<W> &wei){

	if(sen.size() != wei.size()){
		return unordered_map<S,W>(); 
	} 

	unordered_map<S,W> umap(wei.size());

	for (int i = 0; i < sen.size(); ++i)
		umap.insert({sen[i], wei[i]});

	return umap;

}

template <typename S, typename C, typename W>
template <class T>
void Monitor<S, C, W>::buildStatistics(const vector<C> &lab, initializer_list<T> list){

	if(staR.empty()){

		for(auto s: list){
			int i = 0;
			for(auto x: s){
				staR[lab[i]].insert(staR[lab[i]].end(), x);
				i++;}		 
		}

	}else{

		for(const auto x: list){
			int i = 0;
			for(const auto y: x){
				staE[lab[i]].insert(staE[lab[i]].end(), y);
				i++;}		 
		}

	}

}

template <typename S, typename C, typename W>
void Monitor<S, C, W>::showStatistics(){
	
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

/////////////////////////////////////////////////////////////

template <typename S, typename C, typename W>
void Monitor<S, C, W>::checkEvolution(const vector<S>& sen, const vector<C>& clu, const vector<W>& wei, const vector<C> &labels){

	makeHash(sen, clu, wei, labels);

	clusterOverlap();

	TRANS = extTransitions();

	showExtransitions();

	intTransitions();
}

template <typename S, typename C, typename W>
void Monitor<S, C, W>::makeHash(const vector<S> &v1, const vector<C> &v2, const vector<W> &v3, const vector<C> &labels){

	if(v1.size() != v2.size() or v1.size() != v3.size()){
		
	}

	storeLabels(labels); 

	for (int i = 0; i < v1.size(); ++i)
		clusE.insert({v1[i], make_tuple(v2[i],v3[i])});

}

template <typename S, typename C, typename W>
void Monitor<S, C, W>::storeLabels(const vector<C> lab){

	labels = lab;

	sort(labels.begin(),labels.end());

}

/////////////////////////////////////////////////////////////


template <typename S, typename C, typename W>
void Monitor<S, C, W>::clusterOverlap(){

	unordered_map<C,int> lmap = useLabels();
	
	for (auto &c : clusR){
		matrix[c.first] = vector<float>(labels.size()); //saber quantidade de grupos do agrupamento 2
		for (auto &elem : c.second) {
			if(get<0>(clusE[elem]) != -1) matrix[c.first][lmap[get<0>(clusE[elem])]]+= get<1>(clusE[elem]);
		}

	}

	cout << "-----Intersection-----" << endl;
	for (const auto &i : matrix){
		cout << i.first << ": "; 
		for (const auto &x : i.second) cout << x << " ";
		cout << endl;
	}
	cout << endl;

	for (const auto &i : matrix){
		for (auto x = 0; x < i.second.size(); x++){
			matrix[i.first][x] =  matrix[i.first][x]/cluW[i.first];
		}
	}

	cout << "-----Overlapping-----" << endl;
	for (const auto &i : matrix){
		cout << i.first << ": "; 
		for (const auto &x : i.second) cout << x*100 << " ";
		cout << endl;
	}
	cout << endl;

}

template <typename S, typename C, typename W>
unordered_map<C,int> Monitor<S, C, W>::useLabels(){

	unordered_map<C, int> lmap;

	int i = 0;
	for(const auto &x: labels){
		lmap[x] = i;
		i++;
	}

	return lmap;

}

/////////////////////////////////////////////////////////////

template <typename S, typename C, typename W>
Transitions<C> Monitor<S, C, W>::extTransitions(){

	unordered_map<int,C> lmap = hashLabels(labels);

	unordered_map<C, vector<C>> scands;
	set<C> tracks;

	Transitions<C> TRANS;

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
	
	return TRANS;
	
}

template <typename S, typename C, typename W>
unordered_map<int,C> Monitor<S, C, W>::hashLabels(const vector<C> &labels){

	unordered_map<int, C> umap;

	int i = 0;
	for(const auto &x: labels){
		umap[i] = x;
		i++;
	}
	return umap;

}

template <typename S, typename C, typename W>
W Monitor<S, C, W>::sumSplits(const vector<W> &overlaps, const vector<C> &split_cand){

	W sum = 0;

	for(const auto &x: split_cand){
		sum+=overlaps[x];
	}

	return sum;
}

template <typename S, typename C, typename W>
void Monitor<S, C, W>::showExtransitions(){

	cout << "****EXT TRANSITIONS****" << endl << endl; 

	showSurvs();

	showSplits();

	showUnions();

	showDeaths();

	showBirths();
}

/////////////////////////////////////////////////////////////

template <typename S, typename C, typename W>
void Monitor<S, C, W>::intTransitions(){

	cout << "****INT TRANSITIONS****" << endl;
	for(const auto &x: TRANS.getSurvs()){
		cout << get<0>(x) << ": ";

		for(int i = 0; i < staR[get<0>(x)].size(); i++){
			//cout << sta[i] << " = ";
			compare(staE[get<1>(x)][i], staR[get<0>(x)][i]);
			temp[get<0>(x)].insert(temp[get<0>(x)].end(), staE[get<1>(x)][i]/staR[get<0>(x)][i]);
		}

		cout << endl;
	}

	if(qSurvs.size() < winSize){
		qSurvs.push(temp);
	}else{
		qSurvs.pop();
		qSurvs.push(temp);
	}

	temp.clear();
	
	cout << endl;

}

template <typename S, typename C, typename W>
void Monitor<S, C, W>::compare(const float &v1, const float &v2){
	cout << (v1/v2) << " "; 
}

/////////////////////////////////////////////////////////////


template <typename S, typename C, typename W>
void Monitor<S, C, W>::showSurvs(){TRANS.showSurvs();}

template <typename S, typename C, typename W>
void Monitor<S, C, W>::showSplits(){TRANS.showSplits();}

template <typename S, typename C, typename W>
void Monitor<S, C, W>::showUnions(){TRANS.showUnions();}

template <typename S, typename C, typename W>
void Monitor<S, C, W>::showDeaths(){TRANS.showDeaths();}

template <typename S, typename C, typename W>
void Monitor<S, C, W>::showBirths(){TRANS.showBirths();}


template class Monitor<int, int, float>;
template class Monitor<char, int, float>;
template class Monitor<char, char, float>;

template void Monitor<int, int, float>::execute(const vector<int>&, const vector<int>&, const vector<float>&, const vector<int>&, initializer_list<vector<float>> list);
template void Monitor<int, int, float>::buildStatistics(const vector<int>&, initializer_list<vector<float>> list);

template void Monitor<char, char, float>::execute(const vector<char>&, const vector<char>&, const vector<float>&, const vector<char>&, initializer_list<vector<float>> list);
template void Monitor<char, char, float>::buildStatistics(const vector<char>&, initializer_list<vector<float>> list);

template void Monitor<char, int, float>::execute(const vector<char>&, const vector<int>&, const vector<float>&, const vector<int>&, initializer_list<vector<float>> list);
template void Monitor<char, int, float>::buildStatistics(const vector<int>&, initializer_list<vector<float>> list);
