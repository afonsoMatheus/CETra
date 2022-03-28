#include "podclus.hh"

/****************************
 *        CONSTS            *
 ****************************/

/****************************
 * CONSTRUCTORS/DESTRUCTORS *
 ****************************/


 
/**
 * @brief Initializes the class constants and creates the initial clusters.
 *
 * @param ids: The ids of the streams that will compose the initial clusters
 * @param w: The synopsis of the streams that will compose the initial clusters
 * @param initial_clusters: number of initial clusters
 * @param outlier_threshold: value of `kOThresh`
 * @param merge_threshold: value of `kMThresh`
 * @param split_threshold: value of `kSThresh`
 * @param boundary_size: value of `kBoundarySize`
 *
 */
PodClus::PodClus( vector<unsigned int> ids, vector<Window> w, unsigned int initial_clusters, unsigned int outlier_threshold, double merge_threshold, double split_threshold, double boundary_size ) : kOThresh(outlier_threshold), kMThresh(merge_threshold), kSThresh(split_threshold), kBoundarySize(boundary_size)
{
	cs_ = new PodClusClusterSet( kBoundarySize, kBoundarySize*2, kBoundarySize*3 );
	os_ = new PodClusClusterSet( kBoundarySize, kBoundarySize*2, kBoundarySize*3 );
	
	unsigned int cluster_id=0, count = 0;
	
	//create initial clusters
	for ( unsigned int i=0; i < w.size(); ++i )
	{
		if (count == 0 )
			cluster_id = cs_->Create( ids[i], w[i] );
		else
			cs_->Insert( ids[i], w[i], cluster_id );

		++count;
		count %= w.size()/initial_clusters;
	}
	
}

/**
 * @brief Frees the dynamic variables.
 */
PodClus::~PodClus()
{
	delete cs_;
	delete os_;
}

/****************************
 *     GETTERS & SETTERS    *
 ****************************/

/****************************
 *         OTHER            *
 ****************************/

/**
 * @brief Analyzes and adds a stream to the best candidate cluster.
 * 
 * @details (Based on `Pimwadee Chaovalit and Aryya Gangopadhyay. 2009. A method for clustering transient data streams. In Proceedings of the 2009 ACM symposium on Applied Computing (SAC '09). ACM, New York, NY, USA, 1518-1519. DOI=10.1145/1529282.1529622 http://doi.acm.org/10.1145/1529282.1529622`)

 * -# Locate new data in relation to existing cluster boundaries using probabilities if it is an outlier or a strong member of any clusters.
 * -# Assign data to the closest cluster and update cluster synopses, otherwise set this data as an outlier. 
 * -# Detect a cluster to split if the density information falls below a threshold. A splitting cluster’s boundary is reduced by one sigma. Split new data as outliers according to the reduced boundary.
 * -# Merge clusters if the number of strong members belonging to more than one cluster exceeds a threshold.
 *
 * @param s_id: id of the stream to be added
 * @param syn: synopsis of the stream to be added
 */
void PodClus::Add( unsigned int s_id, Estatisticas syn )
{
	long int old_id = -1;
	unsigned int parent_id;
	ClusterType old_type = kNone, parent_type;
	ProximityFlag p_flag;
	
	//Find if a stream has beem added to a cluster or to an outlier before
	if( (old_id = cs_->FindCluster( s_id ) ) != cs_->kNoCluster )
		old_type = kCluster;
	else if ( (old_id = os_->FindCluster( s_id )) != os_->kNoCluster )
		old_type = kOutlier;
	
	//checks proximity with previous associated cluster
	if( old_type == kCluster )
		cs_->CheckBoundary( old_id, syn, p_flag );

	if( p_flag != kOut )
	{
		parent_id = old_id;
		parent_type = old_type;
	}
	else //not in the old cluster
	{
		parent_id = FindNearestCluster( cs_, syn, p_flag );

		if( p_flag != kOut )
		{
			parent_type = kCluster;
		}
		else//not in any clustet -> outliers froup logic begins here
		{
			parent_id = FindNearestCluster( os_, syn, p_flag );
			parent_type = kOutlier;

			if( p_flag == kOut ) //not in any outlier -> create a new outlier
				parent_id = Create( s_id, syn, os_ );
		}
	}

	//cout << "Stream " << s_id << " (" << syn.ToString() << " ) to be added to the " << ( (parent_type==kCluster)?"CLUSTER":"OUTLIER GROUP" ) << " " << parent_id << " ";
	//cout << "with current MinTP=" << ( (parent_type==kCluster)? cs_->MinTP(parent_id): os_->MinTP(parent_id) ) << endl;

	if ( parent_type == kCluster )
	{
		Insert( s_id, syn, cs_, parent_id );
	}
	else
	{
		Insert( s_id, syn, os_, parent_id );
		UpdateOutlierGroup( parent_id );
	}

	CheckSplits();
	CheckMerges();
}

/**
 * @brief Iterates all cluster and checks if any of them needs to be merged, if is that the case, merges clusters.
 *
 * @details Compares all cluster with each other, computes the points in common and merges them if the percentage of common points per the total points is greater than `kMThresh`.
 */
void PodClus::CheckMerges()
{
	unsigned int c1_id, c2_id;
	double in = 0.0, out = 0.0;
	vector<unsigned int> cluster_id_list = cs_->GetIdList();
	ProximityFlag flag;
	Cluster c1, c2;

	for( unsigned int i=0; i < cluster_id_list.size(); ++i )
	{
		c1_id = cluster_id_list[i];

		if( cs_->Exists( c1_id ) ) //check if the cluster has not already been deleted 
			c1 = cs_->Get(c1_id);
		else
			continue;

		for( unsigned int j=i; j < cluster_id_list.size(); ++j )
		{
			c2_id = cluster_id_list[j];

			if ( cs_->Exists( c2_id ) )//check if the cluster has not already been deleted
				c2 = cs_->Get(c2_id);
			else
				continue;

			if ( c1_id != c2_id )
			{
				in = 0.0, out = 0.0;

				//compares stream from c1 with c2
				for ( set<unsigned int>::iterator it = c1.begin(); it != c1.end(); ++it )
				{
					Estatisticas syn = c1.GetStreamSynopsis(*it);
					cs_->CheckBoundary( c2_id, syn, flag );

					if( flag != kOut )
						in += syn.N();
					else
						out += syn.N();
				}
				
				//compares stream from c2 with c1
				for ( set<unsigned int>::iterator it = c2.begin(); it != c2.end(); ++it )
				{
					Estatisticas syn = c2.GetStreamSynopsis(*it);
					cs_->CheckBoundary( c1_id, syn, flag );

					if( flag != kOut )
						in += syn.N();
					else
						out += syn.N();
				}

				if ( in/(in+out) > kMThresh )
					cs_->Merge(c1_id, c2_id);

			}
		}
	}
}

/**
 * @brief Iterates all cluster and checks if any of them needs to be splitted.
 *
 * @details Checks the density of each cluster, if this density is lower than `kSThresh` then split that cluster.
 *
 */
void PodClus::CheckSplits()
{
	vector<unsigned int> cluster_id_list = cs_->GetIdList();

	for( unsigned int i=0; i < cluster_id_list.size(); ++i )
		if( cs_->DensityRatio( cluster_id_list[i] ) < kSThresh )
			cs_->Split( cluster_id_list[i] );
}

/**
 * @brief If the stream is in a different clusterset, removes it from there before create a new cluster.
 *
 * @details If the set to add the stream is the cluster set, removes the stream form the outliers group if it is there, and vice-versa. Then creates a new cluster into the new cluster set.
 *
 * @param stream_id: id of the stream to be added to the new cluster
 * @param stream_syn: synopsis of the stream to be added to the new cluster
 * @param cs: pointer to the cluster set to add the stream
 *
 * @return the id of the new cluster
 */
unsigned int PodClus::Create(unsigned int stream_id, Estatisticas stream_syn, PodClusClusterSet *cs)
{
	if( cs == cs_ )
		os_->EraseStream(stream_id);
	else
		cs_->EraseStream(stream_id);

	return cs->Create( stream_id, stream_syn);
}

/**
 * @brief Find the nearest cluster from the stream.
 *
 * @details Test the distance between the stream and all cluster, and gets the cluster with smaller distance.
 *
 * @param[in] cs: pointer to the cluster set where the searched will be performed
 * @param[in] syn: stream synopsis to be compared to all clusters
 * @param[out] p_flag: flag to indicate the proximity between the nearest cluster and the stream
 */
unsigned int PodClus::FindNearestCluster( PodClusClusterSet *cs, Estatisticas syn, ProximityFlag &p_flag )
{
	vector<unsigned int> ids = cs->GetIdList();
	double min_dist = numeric_limits<double>::max();
	double dist = 0.0;
	ProximityFlag flag;
	unsigned int cluster_id = cs->kNoCluster;

	for ( vector<unsigned int>::iterator it = ids.begin(); it != ids.end(); ++it )
	{
		dist = cs->CheckBoundary( *it, syn, flag );
		//cout << "Distance to " << *it << "=" << dist << endl;
		
		if ( dist < min_dist )
		{
			min_dist = dist;
			p_flag = flag;
			cluster_id = *it;
		}
	}

	return cluster_id;
}

/**
 * @brief If the stream is in a different clusterset, removes from there and adds it to the new cluster.
 *
 * @details If the set to add the stream is the cluster set, removes the stream form the outliers group if it is there, and vice-versa. Then add the stream to the new cluster set.
 
 * @param stream_id: id of the stream to be added to the cluster
 * @param stream_syn: synopsis of the stream to be added to the cluster
 * @param cs: pointer to the cluster set to add the stream
 * @param cluster_id: id of the cluster to receive the stream
 *
 */
void PodClus::Insert(unsigned int stream_id, Estatisticas stream_syn, PodClusClusterSet *cs, unsigned int cluster_id)
{
	if( cs == cs_ )
		os_->EraseStream(stream_id);
	else
		cs_->EraseStream(stream_id);

	cs->Insert( stream_id, stream_syn, cluster_id );
}

/**
 * @brief Checks if a outlier group should become a cluster, if so, transforms it.
 *
 * @details Compares the MinTP value of the outliwer group, if it is greater than kOThresh, then imports the outlier group to the cluster set
 * and remove it from the outlier group set.
 *
 * @param outlier_id: id of the outlier to be checked
 */
void PodClus::UpdateOutlierGroup( unsigned int outlier_id )
{
	if( os_->MinTP( outlier_id ) > kOThresh )
	{
		cs_->Import( os_->Get(outlier_id) ); //imports the cluster from the outliers
		os_->Erase( outlier_id ); //erase from outliers
	}
}


string PodClus::ToStringHeader()
{
	return PodClusClusterSet::ToStringHeader("type");
}

string PodClus::ToString()
{
	stringstream out( stringstream::in | stringstream::out );
	
	out << cs_->ToString("cluster");
	out << os_->ToString("og") << endl;

	return out.str();
}

Clustering PodClus::getClusters()
{
	Clustering C;
	
	C =  cs_->getSensors();

	return C;
}
