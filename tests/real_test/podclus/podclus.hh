#ifndef PODCLUS_H
#define PODCLUS_H

#include<iostream>
#include<limits>
#include<sstream>
#include<string>
#include<map>

#include "estatisticas.hh"
#include "cluster.hh"
#include "window.hh"

#include "podclusclusterset.hh"

using namespace std;

/**
 * @class PodClus podclus.hh
 *
 * @brief Implements the Pod-Clus algorithm.
 *
 * @details This class holds the content to test the Pod-Clus algorithm. Adding data is using the `Add` method and `ToString` to check the current output.
 *
 * @author Matheus E. Ramos
 * @version 0.4
 * @date September 18 2014
 *
 */
class PodClus{
	private:
		const unsigned int kOThresh; /**< Threshold of the minimum times a stream remains at the same outlier group after receive new data before it becomes a cluster */
		const double kMThresh; /**< Threshold of the acceptable percentage of points in poisition `kFar` from the centroid, before clusters needs to merge */
		const double kSThresh; /**< Threshold of the acceptable percentage of far from the centroid, before the cluster needs to split */
		const double kBoundarySize; /**< Defines how many standard deviations from the mean a point will be considered near the centroid of the cluster */

		/**
		 * @brief Defines the possible types of storage.
		 */
		enum ClusterType{
			kCluster, /**< Data belongs to a cluster */
			kOutlier, /**< Data belongs to an outlier group */
			kNone			/**< Not defined where data belongs */
		};

		PodClusClusterSet *cs_; /**< Cluster Set **/
		PodClusClusterSet *os_; /**< Outlier Group Set **/
		
		void CheckMerges();
		void CheckSplits();
		unsigned int FindNearestCluster(PodClusClusterSet *, Estatisticas, ProximityFlag &);
		void Insert(unsigned int, Estatisticas, PodClusClusterSet *, unsigned int);
		unsigned int Create(unsigned int, Estatisticas, PodClusClusterSet *);
		void UpdateOutlierGroup(unsigned int);

	public:
		PodClus(vector<unsigned int>, vector<Window>, unsigned int, unsigned int, double, double, double);
		~PodClus();

		void Add(unsigned int, Estatisticas);

		string ToStringHeader();
		string ToString();

		Clustering getClusters();
};

#endif
