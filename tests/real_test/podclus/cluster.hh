#ifndef CLUSTER_H
#define CLUSTER_H

#include<iostream>
#include<map>
#include<set>
#include<sstream>
#include<string>

#include "estatisticas.hh"

using namespace std;



/**
 * @class Cluster cluster.hh
 *
 * @brief Implement a functional cluster
 *
 * @details This implementation of a cluster can hold a set of stream ids and its statistics.
 * It generates its own synopsis based on the junction of its streams statistics.
 * It also supports adding a already existing stream, it just updates the stream statistics.
 *
 * @author Matheus E. Ramos
 * @version Revision: 0.2
 * @date September 17 2014
 * 
 */
class Cluster: public set<unsigned int>
{
	private:
		map< unsigned int, Estatisticas > stream_syn_;

	public:
		void Insert(unsigned int, Estatisticas);
		void Erase(unsigned int);
		bool Find(unsigned int);
		Estatisticas GetStreamSynopsis(unsigned int);
		Estatisticas Synopsis();

		static string ToStringHeader();
		string ToString();

		Clustering getSensors(Clustering &, unsigned int);
};

#endif
