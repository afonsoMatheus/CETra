#include "clusterset.hh"

/****************************
 *        CONSTS            *
 ****************************/
const short int ClusterSet::kNoCluster = -1; //flag to sinalize no cluster associated

/****************************
 * CONSTRUCTORS/DESTRUCTORS *
 ****************************/
ClusterSet::ClusterSet(){
	last_cluster_id_ = 0;
}

/****************************
 *     GETTERS & SETTERS    *
 ****************************/

/****************************
 *         OTHER            *
 ****************************/

/**
 * @brief Creates a new cluster and adds the stream info to it
 *
 * @details Generates a brand new cluster id, remove the stream if if belongs to another cluster and add it to the new cluster.
 *
 * @param stream_id: id of the stream to be added to the new cluster
 * @param stream_syn: synopsis of the stream to be added to the new cluster
 *
 * @return the id of the new cluster
 */
unsigned int ClusterSet::Create(unsigned int stream_id, Estatisticas stream_syn)
{
	unsigned int new_cluster_id = ++last_cluster_id_; //get the next available id
	//long int current_cluster_id = FindCluster( stream_id ); //check if the stream is already part of a cluster


	//if( current_cluster_id != kNoCluster ) //if the stream belongs to a cluster, erase from the cluster
		//cs_[current_cluster_id].Erase( stream_id );
	EraseStream( stream_id ); //check if the stream is already part of a cluster and erases from there

	Cluster c; //new cluster
	c.Insert( stream_id, stream_syn );

	cs_[new_cluster_id] = c; //join the id and cluster
	
	return new_cluster_id;
}

/**
 * @brief Erases a cluster.
 *
 * @details Erases the cluster information from the map of clusters.
 *
 * @param cluster_id: id of the cluster to be erased
 *
 * @note the cluster must exists
 */
void ClusterSet::Erase( unsigned int cluster_id )
{
	cs_.erase(cluster_id);
}

/**
 * @brief Removes a stream if it exists in any cluster.
 *
 * @details Searches the cluster that the stream belons, if there is a cluster, removes the stream from it.
 *
 * @param: id of the stream to be removed
 *
 * @returns id of the cluster that the stream belonged
 */
long int ClusterSet::EraseStream( unsigned int stream_id )
{
	long int cluster_id = FindCluster( stream_id );

	if ( cluster_id != kNoCluster )
	{
		cs_[cluster_id].Erase( stream_id );
		if( cs_[cluster_id].empty() )
			Erase(cluster_id);
	}
	
	return cluster_id;
}

/**
 * @brief Checks if a cluster exists.
 *
 * @details Checks the cluster map, if a cluster with the key `cluster_id` exists returns true.
 *
 * @retvalue TRUE: cluster exists
 * @retvalue FALSE: cluster does not exist
 */
bool ClusterSet::Exists( unsigned int cluster_id )
{
	if( cs_.count( cluster_id ) == 0 )
		return false;
	return true;
}

/**
 * @brief Finds the cluster id that owns the stream.
 *
 * @details Loops the clusters and searches the cluster that owns the stream id passed by parameter.
 *
 * @param stream_id: id of the stream to be found
 *
 * @returns an unsigned int meaning id of the cluster that owns the stream
 * @retvalue kNoCluster if the stream does not belong to any cluster
 */
long int ClusterSet::FindCluster( unsigned int stream_id )
{
	for( map<unsigned int,Cluster>::iterator it = cs_.begin(); it != cs_.end(); ++it )
	{
		if ( it->second.Find(stream_id) ) {
			//cout << stream_id << " pertence ao cluster " << it->first << endl;
			return it->first;
		}
	}
	return kNoCluster;
}

/**
 * @brief Gets a cluster.
 *
 * @details Returns a cluster based on its id.
 *
 * @note The cluster must exist.
 *
 * @param cluster_id: id of the cluster to be get
 *
 * @return The desired cluster object
 */
Cluster ClusterSet::Get(unsigned int cluster_id)
{
	return cs_[ cluster_id ];
}

/**
 * @brief Get a list of ids of all current clusters.
 *
 * @details Iterates map of cluster and store all ids in a vector.
 *
 * @returns vector containing all clusters ids
 */
vector<unsigned int> ClusterSet::GetIdList()
{
	vector<unsigned int> ids;

	for( map<unsigned int,Cluster>::iterator it = cs_.begin(); it != cs_.end(); ++it )
		ids.push_back( it->first );

	return ids;
}

/**
 * @brief Imports a cluster to the cluster set.
 *
 * @details Creates a brand new cluster id. Searches for the stream in another clusters, to remove from there. Then adds the new cluster to the cluster set.
 *
 * @param c: cluster to be imported
 *
 * @returns id of the new cluster
 */
unsigned int ClusterSet::Import( Cluster c )
{
	unsigned int new_cluster_id = ++last_cluster_id_; //get the next available id

	for ( set<unsigned int>::iterator it = c.begin(); it != c.end(); ++it )
		EraseStream(*it); //if the stream belongs to a cluster, erase from the cluster

	cs_[last_cluster_id_] = c;

	return new_cluster_id;
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
void ClusterSet::Insert( unsigned int stream_id, Estatisticas stream_syn, unsigned int cluster_id )
{
	EraseStream(stream_id); //if the stream belongs to a cluster, erase from the cluster
	cs_[cluster_id].Insert( stream_id, stream_syn ); //put on the cluster
}

/**
 * @brief Merge 2 clusters.
 *
 * @details Put all stream of the second cluster in the first cluster and then erases the second cluster.
 *
 * @param c_id1: id of the first cluster to be merged
 * @param c_id2: id of the second cluster to be merged
 */
void ClusterSet::Merge( unsigned int c_id1, unsigned int c_id2)
{
	for( set<unsigned int>::iterator it = cs_[ c_id2 ].begin(); it != cs_[ c_id2 ].end(); ++it ) //add c_id2 streams on c_id1
		cs_[ c_id1 ].insert( *it );
		//Insert( *it, cs_[c_id2].GetStreamSynopsis(*it), c_id1 );

	Erase( c_id2 ); //erase cluster c_id2
}


/**
 * @brief Update stream synopsys.
 *
 * @details Search for the stream cluster and then updates its synopsis.
 *
 * @param stream_id: id of the stream whose cluster will be updated
 * @param syn: statistics to be updated
 */
void ClusterSet::Update(unsigned int stream_id, Estatisticas syn)
{
	long int cluster_index = FindCluster( stream_id );

	if( cluster_index != kNoCluster )
		cs_[cluster_index].Insert( stream_id, syn );
	//else
		//cout << "Stream " << stream_id << " does not have cluster." << endl;

}

string ClusterSet::ToStringHeader()
{
	stringstream out( stringstream::in | stringstream::out );

	out << "\"cluster id\"," << Cluster::ToStringHeader();

	return out.str();
}
	

string ClusterSet::ToString()
{
	stringstream out( stringstream::in | stringstream::out );
	//out << "print " << endl;
	for( map<unsigned int, Cluster>::iterator it = cs_.begin(); it != cs_.end(); ++it ){
		//out << "print 2" << endl;
		out << it->first << "," << it->second.ToString() << endl;

	}
	//out << "Last cluster is used" << last_cluster_id_ << ", actually " << cs_[last_cluster_id_].ToString() << endl;
	return out.str();
}

