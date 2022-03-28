#include "cluster.hh"

/****************************
 *        CONSTS            *
 ****************************/

/****************************
 * CONSTRUCTORS/DESTRUCTORS *
 ****************************/

/****************************
 *     GETTERS & SETTERS    *
 ****************************/

/****************************
 *         OTHER            *
 ****************************/

/**
 * @brief Erases a stream from the cluster.
 *
 * @details Erases from the set and from the synopsis map
 *
 */
void Cluster::Erase( unsigned int stream_id )
{
	stream_syn_.erase(stream_id);
	erase(stream_id);
}

/**
 * @brief Searches the stream in the cluster.
 *
 * @retval TRUE if the stream belongs to the cluster
 * @retval FALSE if the stream does not belong to the cluster
 */
bool Cluster::Find( unsigned int stream_id )
{
	if( count(stream_id) != 0 )
		return true;
	return false;
}

/**
 * @brief Returns the synopsis of a stream.
 *
 * @param stream_id: id of the stream to have the synopsis retrieved
 *
 * @return Synopsis of the desired stream.
 */
Estatisticas Cluster::GetStreamSynopsis( unsigned int stream_id )
{
	return stream_syn_[stream_id];
}

/**
 * @brief Inserts a new stream in the cluster.
 *
 * @details If the stream already belongs to the cluster, then remove old information and add the stream again.
 *
 */
void Cluster::Insert( unsigned int stream_id, Estatisticas syn )
{
	if ( Find(stream_id) ) //already mine?
		Erase( stream_id );

	insert( stream_id );
	stream_syn_[stream_id] = syn;
}

/**
 * @brief Returns the cluster synopsis.
 *
 * @details Fuses all streams synopsis to generate the cluster synopsis.
 *
 * @return cluster statistical synopsis
 */
Estatisticas Cluster::Synopsis()
{
	Estatisticas e;

	for ( map<unsigned int,Estatisticas>::iterator it = stream_syn_.begin(); it != stream_syn_.end(); ++it )
		e.Fusion( it->second );

	return e;
}

string Cluster::ToStringHeader()
{
	stringstream out( stringstream::in | stringstream::out );

	out << "streams," << Estatisticas::ToStringHeader();

	return out.str();
}

string Cluster::ToString()
{
	stringstream out( stringstream::in | stringstream::out );
	
	out << "\"";
	for ( set<unsigned int>::iterator it = begin(); it != end(); ++it )
	{
		if ( it != begin() )
			out << ",";
		out << *it;
	}
	out << "\"";

	out << "," << Synopsis().ToString();

	//print stations
	out << endl;
	
	/*
	//if( size() != stream_syn_.size() )
		//cout << "Set size = " << size() << "; Map size = " << stream_syn_.size() << ";" << endl;
	for ( map<unsigned int,Estatisticas>::iterator it = stream_syn_.begin(); it != stream_syn_.end(); ++it )
	{
		if ( it != stream_syn_.begin() ) out << endl;
		out << it->first << "," << it->second.ToString();
	}*/

	return out.str();
}

Clustering Cluster::getSensors(Clustering &C, unsigned int c)
{
	
	for ( set<unsigned int>::iterator it = begin(); it != end(); ++it )
	{
		C.sensors.push_back(*it);
		C.clusters.push_back(c);
		C.weights.push_back(1.0);

	}

	Synopsis().ToMap(C);

	for ( map<unsigned int,Estatisticas>::iterator it = stream_syn_.begin(); it != stream_syn_.end(); ++it )
	{
		C.values[0].push_back(it->second.getMedia());
		C.values[1].push_back(it->second.getDesPad());
	}

	return C;
}
