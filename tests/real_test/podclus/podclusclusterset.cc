#include "podclusclusterset.hh"


/****************************
 *        CONSTS            *
 ****************************/

/****************************
 * CONSTRUCTORS/DESTRUCTORS *
 ****************************/

PodClusClusterSet::PodClusClusterSet( double first_bound, double second_bound, double third_bound) : kFirstBound(first_bound), kSecondBound(second_bound), kThirdBound(third_bound) {}

/****************************
 *     GETTERS & SETTERS    *
 ****************************/

/****************************
 *         OTHER            *
 ****************************/

/**
 * @brief Calculate the disce of a stream from a cluster and decides their proximity.
 *
 * @details defines the proximity based on the defined boundaries (`KFirstBound`, `kSecondBound`, `kThirdBound`) and then calculates the distance using the method `Distance`.
 *
 * @param[in] cluster_id: id of the cluster to be compared
 * @param[in] stream_syn: synopsis of the stream to be compared
 * @param[out] p_flag: flag to define the proximity of the stream and the cluster
 *
 * @returns distance from the cluster
 */
double PodClusClusterSet::CheckBoundary( unsigned int cluster_id, Estatisticas stream_syn, ProximityFlag &p_flag)
{
	Estatisticas cluster_syn = cs_[cluster_id].Synopsis();
	double bound = bound_[cluster_id];
//cout << "chk bound" << endl;
	if( stream_syn.media() > cluster_syn.media() - kFirstBound * cluster_syn.desvio_padrao() && 
			stream_syn.media() < cluster_syn.media() + kFirstBound * cluster_syn.desvio_padrao() ) // m-dp < s.m < m+dp -> close to centroid
		p_flag = kNear;
	else if( stream_syn.media() > cluster_syn.media() - bound * cluster_syn.desvio_padrao() && 
					 stream_syn.media() < cluster_syn.media() + bound * cluster_syn.desvio_padrao()  ) // m-bound*dp < s.m < m-dp OR m+dp < s.m < m+bound*dp --medium dist from centroid
		p_flag = kFar;
	else //outlier
		p_flag = kOut;

	return Distance( cluster_syn, stream_syn );

}

/**
 * @brief Creates a new cluster and adds the stream info into it
 *
 * @details Generates a brand new cluster id, remove the stream if if belongs to another cluster and add it to the new cluster, then add bound information.
 *
 * @param stream_id: id of the stream to be added to the new cluster
 * @param stream_syn: synopsis of the stream to be added to the new cluster
 *
 * @return the id of the new cluster
 */
unsigned int PodClusClusterSet::Create(unsigned int stream_id, Estatisticas stream_syn)
{
	unsigned int cluster_id = ClusterSet::Create(stream_id, stream_syn);

	bound_[cluster_id] = kThirdBound;
	min_tp_[cluster_id][stream_id] = 0;

	return cluster_id;
}

/**
 * @brief Returns the ratio of the points near the centroid per the total points.
 *
 * @details Gets the synopsis of all stream of the cluster and analyzes them to check if they are near of the centroid. The sum of points near the centroid is divided per the total of 
 * the points in the cluster.
 *
 * @param cluster_id: id to the cluster to have the density analyzed
 */
double PodClusClusterSet::DensityRatio( unsigned int cluster_id )
{
	double in = 0.0, out = 0.0;
	ProximityFlag flag;

	for ( set<unsigned int>::iterator it = cs_[cluster_id].begin(); it != cs_[cluster_id].end(); ++it )
	{
		Estatisticas syn = cs_[cluster_id].GetStreamSynopsis(*it);
		CheckBoundary( cluster_id, syn, flag );
		
		if ( flag == kNear )
			in += syn.N();
		else
			out += syn.N();
	}

	return ( in / (in + out) );

}

/**
 * @brief Calculates the distance between cluster and a stream. It is used the Bhattacharyya distance (see <http://en.wikipedia.org/wiki/Bhattacharyya_distance>)
 *
 * @details It is used Bhattcharyya distance instead of the Mahalanobis based equation sugested by Chaovalit, because it is more tolerant with standard deviations equal to zero.

 * @param cluster_syn: cluster synopsis
 * @param stream_syn: stream synopsis
 *
 * @returns Distance between a cluster and a stream based on Chaovalit equation.
 */
double PodClusClusterSet::Distance( Estatisticas cluster_syn, Estatisticas stream_syn )
{
	double d = 0.0, desp;

	d = pow( stream_syn.media() - cluster_syn.media() , 2 );
	desp = stream_syn.desvio_padrao();

	if (desp == 0)
		desp = 1;
	//cout << "DESVP = " << desp << endl;
	d /= pow( desp, 2 );

	//if( s.desvio_padrao() != 0.0 )
		 return d ;///= pow( stream_syn.desvio_padrao(), 2 );
	//else
		//return 0.0;
	/*
	double d = 0.0, e = 0.0;

	d = 2 * cluster_syn.desvio_padrao() * stream_syn.desvio_padrao();
	d /= pow(cluster_syn.desvio_padrao(),2) + pow(stream_syn.desvio_padrao(),2);
	d = sqrt(d);

	e = pow( cluster_syn.media() - stream_syn.media(), 2 );
	e /= pow(cluster_syn.desvio_padrao(),2) + pow(stream_syn.desvio_padrao(),2);
	e /= -4;
	e = exp(e);

	d *= e;
	d = 1 - d;

	return d;
	*/
	/*double d = 0.0, b1 = 0.0, b2 = 0.0;

	b1 = pow(stream_syn.desvio_padrao(),4) / pow(cluster_syn.desvio_padrao(),4);
	b1 += pow(cluster_syn.desvio_padrao(),4) / pow(stream_syn.desvio_padrao(),4);
	b1 += 2;
	b1 /= 4;
	b1 = log(b1);
	b1 /= 4;

	b2 = pow(stream_syn.media() - cluster_syn.media(), 2);
	b2 /= pow(stream_syn.desvio_padrao(),4) + pow(cluster_syn.desvio_padrao(),4);
	b2 /= 4;

	d = b1 + b2;
	*/

	return d;
}


/**
 * @brief Erases a cluster.
 *
 * @details Erases the cluster information from the map of clusters and from the bound information.
 *
 * @param cluster_id: id of the cluster to be erased
 *
 * @note the cluster must exists
 */
void PodClusClusterSet::Erase( unsigned int cluster_id )
{
	ClusterSet::Erase(cluster_id);
	bound_.erase(cluster_id);
	min_tp_.erase(cluster_id);
}


/**
 * @brief Inserts a stream in a cluster.
 *
 * @details Removes the streams if it belongs to another cluster and add it to the desired cluster.
 *
 * @note the cluster must exist
 *
 * @param stream_id: id of the stream to be added to the cluster
 * @param stream_syn: synopsis of the stream to be added to the cluster
 * @param cluster_id: id of the cluster to receive the stream
 */
void PodClusClusterSet::Insert( unsigned int stream_id, Estatisticas stream_syn, unsigned int cluster_id )
{
	unsigned int curr_cluster_id = FindCluster( stream_id );

	ClusterSet::Insert( stream_id, stream_syn, cluster_id );

	if( curr_cluster_id == cluster_id )
	{
		if( min_tp_[cluster_id].count(stream_id) != 0 )
			min_tp_[cluster_id][stream_id] += 1;
		else
			min_tp_[cluster_id][stream_id] = 1;
	}
	else
		min_tp_[cluster_id][stream_id] = 1;

}

/**
 * @brief Merge 2 clusters.
 *
 * @details Put all stream of the second cluster in the first cluster and then erases the second cluster and the bound information of the second cluster.
 *
 * @param c_id1: id of the first cluster to be merged
 * @param c_id2: id of the second cluster to be merged
 */
void PodClusClusterSet::Merge( unsigned int c_id1, unsigned int c_id2)
{
	ClusterSet::Merge(c_id1, c_id2);
	bound_.erase(c_id2);
}


/**
 * @brief Returns the sum of insertions that each stream remained in the cluster.
 *
 * @details If a stream is inserted in a cluster its min_tp = 1, each time a new data of the stream arrives and the stream remains in the same cluster,
 * then its min_tp is increased by one. This methos return the sum of min_tp's of all streams of the cluster.
 *
 * @param cluster_id: id of the cluster to have the min_tp calculated
 *
 * @returns the sum of min_tp's (how many times the stream remained in the cluster after data inclusion) of all streams in the cluster
 */
unsigned int PodClusClusterSet::MinTP( unsigned int cluster_id )
{
	unsigned int count_tp = 0;
	for( map< unsigned int, unsigned int >::iterator it = min_tp_[cluster_id].begin(); it != min_tp_[cluster_id].end(); ++it )
		count_tp += it->second;

	return count_tp;
}

/**
 * @brief Diminishes the bound so new points far from the centroids enter in another cluster.
 *
 * @details The default value of the bound is `kThirdBound`, the difference is that points that now falls after the second bound will be taken as outsiders of the cluster.
 *
 */
void PodClusClusterSet::Split( unsigned int cluster_id)
{
	bound_[cluster_id] = kSecondBound;
}

string PodClusClusterSet::ToStringHeader( string type_name)
{
	stringstream out( stringstream::in | stringstream::out );

	out << "\"" << type_name << "\"," << ClusterSet::ToStringHeader();

	return out.str();
}

string PodClusClusterSet::ToString( string type)
{
	stringstream out( stringstream::in | stringstream::out );
	
	for( map<unsigned int, Cluster>::iterator it = cs_.begin(); it != cs_.end(); ++it )
		out << "\"" << type << "\"," << it->first << "," << it->second.ToString() << endl;
	return out.str();
}

Clustering PodClusClusterSet::getSensors(){

	Clustering C;
	
	for( map<unsigned int, Cluster>::iterator it = cs_.begin(); it != cs_.end(); ++it ){

		it->second.getSensors(C, it->first);
	}

	return C;
}
