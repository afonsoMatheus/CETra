#ifndef CLUSTERSET_H
#define CLUSTERSET_H

#include<iostream>
#include<map>
#include<set>
#include<sstream>
#include<string>
#include<vector>

#include "cluster.hh"
#include "estatisticas.hh"

using namespace std;

/**
 *
 */
class ClusterSet{
	protected:
		map<unsigned int, Cluster> cs_;
		long int last_cluster_id_;

	public:
		static const short int kNoCluster; //flag to sinalize no cluster associated

		ClusterSet();

		unsigned int Create(unsigned int, Estatisticas);
		void Erase(unsigned int);
		long int EraseStream(unsigned int);
		bool Exists(unsigned int);
		long int FindCluster(unsigned int);
		Cluster Get(unsigned int);
		vector<unsigned int> GetIdList();
		unsigned int Import(Cluster);
		void Insert(unsigned int, Estatisticas, unsigned int);
		void Merge(unsigned int, unsigned int);
		void Split(unsigned int);
		void Update(unsigned int, Estatisticas);

		static string ToStringHeader();
		string ToString();

};

#endif
