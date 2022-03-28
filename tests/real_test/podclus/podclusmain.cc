#include<cmath>
#include<cstdlib>
#include<iostream>
#include<string>
#include<vector>

#include "estatisticas.hh"
#include "filelister.hh"
#include "stream.hh"
#include "window.hh"

#include "podclus.hh"

#include "/home/afonso/Documentos/Dissertação/Masters Degree/stream_solution/CETra.h"
#include "/home/afonso/Documentos/Dissertação/Masters Degree/monic/C++/Monic.cpp"
#include <chrono>

using namespace std;
using namespace std::chrono;

const string kInputFormatMessage("NAME\nPOD-Clus - Data mining clustering algorithm\n\nSYNOPSIS\npod.out [OUTLIER THRESHOLD] [BOUNDARY SIZE] [WINDOW SIZE] [SHIFT SIZE] [INPUT FOLDER] [COLUMN INDEX]\n");
const double kSThresh = 0.4868; //Chaovalit, 2009 suggested value
const double kMThresh = 0.1506; //Chaovalit, 2009 suggested value
const double kDecayFactor = exp2(-0.1); //Chaovalit, 2009 suggested value (p.77)

/**
 * @brief Main file to test the Pod-Clus algorithm.
 *
 * @param argc: number of params
 * @param argv[0]: program name
 * @param argv[1]: OUTLIER THRESHOLD: threshold containing the maximum number of stream one outlier can have before become a cluster
 * @param argv[2]: BOUNDARY SIZE: inside how many standard deviations from the mean a point will be considered near the centroid of the cluster
 * @param argv[3]: WINDOW SIZE: size of the data window of a stream
 * @param argv[4]: SHIFT SIZE: size to shift data to generate another window
 * @param argv[5]: INPUT FOLDER: folder containing the inputs files
 * @param argv[6]: COLUMN INDEX: column index of the value in the csv file
 *
 */
int main( int argc, char *argv[] )
{
	unsigned int outlier_threshold, iterations = 0;
	unsigned int window_size=90, shift_size=30, column_index=2;
	double boundary_size;
	string input_dir, type_process;
	vector<string> files;
	vector<Stream *> streams;
	vector<Window> w;
	vector<unsigned int> ids;
	FileLister *lister;

	////////////////////
	/////  PARAMS //////
	////////////////////
	if( argc > 6 )
	{
		outlier_threshold = atoi( argv[1] );
		boundary_size = atof( argv[2] );
		window_size = atoi( argv[3] );
		shift_size = atoi( argv[4] );
		input_dir = argv[5];
		column_index = atoi( argv[6] );
	}
	else
	{
		cerr << kInputFormatMessage << endl;
		exit( EXIT_FAILURE );
	}
	
	lister = new FileLister( input_dir.c_str() );
	files = lister->Files();
	//cout << lister->ToString() << endl;

	//creates a stream to each file
	for( unsigned int i=0; i < files.size(); ++i )
		streams.push_back( new Stream( files[i], window_size, column_index) );

	////////////////////
	///// PODCLUS //////
	////////////////////

	//Build initial input
	for ( unsigned int i=0; i < streams.size(); ++i )
	{
		//cout << "Stream " << streams[i]->Id() << endl;
		if ( streams[i]->W().N() > 0 )
		{
			w.push_back( streams[i]->W() ); //store window
			ids.push_back( streams[i]->Id() ); //store id
		}

		streams[i]->Shift( shift_size );
	}

	PodClus podclus( ids, w, 15, outlier_threshold, kMThresh, kSThresh, boundary_size );//streams.back()->Synopsis(), outlier_threshold );
	//cout << podclus.ToStringHeader() << endl << podclus.ToString();

	CETra<unsigned int, unsigned int> mor(1);
	mor.configSizeLimit(0.45);
	mor.configNewIntStatistic(0.45, "N");
	mor.configNewIntStatistic(0.45, "Mean");
	mor.configNewIntStatistic(0.45, "Std");
	mor.configNewIntStatistic(0.45, "Sum");
	mor.configNewIntStatistic(0.45, "SS");

	Clustering C_u;

	double monic_time = 0.0;
	double cetra_time = 0.0;

	vector<float> lim = {0.45,0.45,0.45,0.45,0.45};
	vector<string> nam = {"N", "Mean", "Std", "Sum", "SS"};

	while( !streams.empty() )
	{

		auto start = high_resolution_clock::now();
		
		for( long int i=streams.size() -1; i >= 0; --i )
		{
			if ( streams[i]->HasNew() )
			{
				//cout << "Data coming from STREAM " << streams[i]->Id() << endl;
				//streams[i]->Next();
				streams[i]->Multiply( kDecayFactor );
				streams[i]->Shift( shift_size ); //1 week

				if ( streams[i]->W().N() > 0 )
					podclus.Add( streams[i]->Id(), streams[i]->Synopsis() );

				//cout << streams[i]->Synopsis().ToString() << endl;

				//cout << "LALALA" << endl << flush;
			}
			else //deletes the stream of each file
			{
				delete streams[i];
				streams.erase( streams.begin() + i );
			}
			//cout << podclus.ToStringHeader() << endl << podclus.ToString();
		}

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);

		cout << "################# Iteration " << iterations << " #################" << endl;
		cout << "PodClus Time: " << duration.count() << endl << endl;
		//cout << podclus.ToStringHeader() << endl << podclus.ToString();

		if(iterations == 0){

			C_u = podclus.getClusters();

			mor.execute(C_u.sensors, C_u.clusters, C_u.weights, {C_u.inter[0], C_u.inter[1],C_u.inter[2],C_u.inter[3],C_u.inter[4]});

			C_u.exportClustering(iterations);
		
		}else{

			Clustering C_a = C_u;

			C_u = podclus.getClusters();

			C_u.exportClustering(iterations);

			/////////////////////////////////

			start = high_resolution_clock::now();

			Monic(C_a, C_u, lim, nam);

			stop = high_resolution_clock::now();
			duration = duration_cast<microseconds>(stop - start);

			cout << "Monic Time: " << duration.count() << endl << endl;
			monic_time = monic_time + duration.count();

			cout << "---------------------------------------------" << endl;

			/////////////////////////////////

			start = high_resolution_clock::now();

			mor.execute(C_u.sensors, C_u.clusters, C_u.weights, {C_u.inter[0], C_u.inter[1],C_u.inter[2],C_u.inter[3],C_u.inter[4]});

			stop = high_resolution_clock::now();
			duration = duration_cast<microseconds>(stop - start);

			cout << "CETra Time: " << duration.count() << endl << endl;
			cetra_time = cetra_time + duration.count();

			/////////////////////////////////

		}

		++iterations;
	}

	cout << "Monic Total Time: " << monic_time << endl;
	cout << "CETra Total Time: " << cetra_time << endl;

	delete lister;

	return EXIT_SUCCESS;
}
