#include <vector>
#include <tuple>
#include <unordered_map>
#include <iostream>

using namespace std;

class Transitions{

	public:

		vector<int> deaths;
		//vector<int> tracked;
		vector<int> births;
		unordered_map<int, vector<int> > absors;
		unordered_map<int, vector<int> > splits;
		vector<tuple<int,int>> survs;

		Transitions(){

		};

		void showDeaths(){
			cout << "Mortes" << endl;
			for(const auto &x : deaths){
				cout << x << " -> DEATH " << endl;
			}
			cout << endl;
		}

		void showSplits(){
			cout << "Separações" << endl; 
			for(const auto &x: splits){
				cout << x.first << " -> ";
				for(const auto &y: x.second) {
					cout << y << " ";
				};
				cout << endl;

			}
			cout << endl;
		}

		void showBirths(){
			cout << "Nascimentos" << endl;
			for(const auto &x : births){
				cout << "BIRTHS -> " << x << endl;
			}
			cout << endl;
		}

		void showSurvs(){
			cout << "Sobreviventes" << endl;
			for(const auto &x : survs){
				cout << get<0>(x) << " -> " << get<1>(x)<< endl;
			}
			cout << endl;
		}

		void showAbsors(){

			cout << "Uniões" << endl;
			for(const auto &x: absors){
				//if(x.second.size() > 1){
					for(const auto &y: x.second) {
						cout << y << " ";
					}
					cout << "-> " << x.first << endl;
				//}
				
			}
			cout << endl;
		}

};