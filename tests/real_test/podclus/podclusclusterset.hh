#ifndef PODCLUSCLUSTERSET_H
#define PODCLUSCLUSTERSET_H

#include<cmath>
#include<iostream>
#include<map>
#include<sstream>

#include "clusterset.hh"

using namespace std;

enum ProximityFlag{
	kNear,
	kFar,
	kOut
};

class PodClusClusterSet: public ClusterSet{
	private:
		const double kFirstBound; //how many standard deviations far from the mean it will be considerate _near_ the centroid
		const double kSecondBound; //the distance between `kFirstBound` and `kSecondBound` standard deviations from the centroid, will be considerated _far_ from the centroid _AFTER SPLITTING_
		const double kThirdBound; //the distance between `kFirstBound` and `kThirdBound` standard deviations from the centroid, will be considerated _far_ from the centroid _BEEFORE SPLITTING_, more than that it will be considered an _outlier_

		map<unsigned int, double> bound_;
		map<unsigned int, map<unsigned int, unsigned int> > min_tp_; //counts which stream continually belongs to a cluster min_tp_[cluster_id][stream_id]
		double Distance(Estatisticas, Estatisticas);
	public:

		PodClusClusterSet(double, double, double);
		double CheckBoundary(unsigned int, Estatisticas, ProximityFlag &);
		unsigned int Create(unsigned int, Estatisticas);
		double DensityRatio(unsigned int);
		void Erase(unsigned int);
		void Insert(unsigned int, Estatisticas, unsigned int);
		void Merge(unsigned int, unsigned int);

		unsigned int MinTP(unsigned int);
		void Split(unsigned int);

		static string ToStringHeader(string);
		string ToString(string);

		Clustering getSensors();
};

#endif
