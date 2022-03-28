#ifndef ESTATISTICA_H
#define ESTATISTICA_H

#include<cmath>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<map>
#include<string>
#include<fstream>

using namespace std;

struct Clustering{

	vector<unsigned int> sensors;
	vector<unsigned int> clusters;
	vector<float> weights;
	map<unsigned int, vector<float>> inter;
	map<unsigned int, vector<double>> values;


	void exportClustering(const int &iter){

		ofstream output("/home/afonso/Documentos/Dissertação/Masters Degree/tests/real_test/podclus/clusterings/clustering_" + to_string(iter));

		for(const auto &x: sensors){ 
			output << x << " ";
		}	
		output << endl;

		for(const auto &x: clusters){ 
			output << x << " ";
		}	
		output << endl;

		for(const auto &x: values[0]){ 
			output << x << " ";
		}	
		output << endl;

		for(const auto &x: values[1]){ 
			output << x << " ";
		}	
		output << endl;

		output.close();
	}

};

/**
 * Class will hold statistical information and will 
 * performe basic statistical operations.
 */
class Estatisticas{
	private:
		double soma_, soma_quadrados_, media_, desvio_padrao_;
		unsigned long int n_;

		void Update();

	public:
		Estatisticas();
		Estatisticas( double, double, double, double );

		double soma();
		double soma_quadrados();
		double media();
		double desvio_padrao();
		unsigned long int N();

		void Add(double);
		void Clear();
		void Remove(double);
		void Fusion(Estatisticas);
		//void Fade(double);

		static string ToStringHeader();
		string ToString();

		void ToMap(Clustering &);

		double getMedia(){
			return media_;
		}

		double getSoma(){
			return soma_;
		}

		double getDesPad(){
			return desvio_padrao_;
		}

		double getSS(){
			return soma_quadrados_;
		}

};

#endif
