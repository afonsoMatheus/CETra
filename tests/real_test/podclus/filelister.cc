#include "filelister.hh"

/****************************
 *        CONSTS            *
 ****************************/

/****************************
 * CONSTRUCTORS/DESTRUCTORS *
 ****************************/

FileLister::FileLister(string directory)
{
	DIR *dir;
	struct dirent *ent;
	struct stat st;
	unsigned char isFile =0x4;
	//cout << "Opening directory " << directory << endl;

	if( ( dir = opendir ( directory.c_str() ) ) != NULL ) //open directory
	{
		//remove . and .. directories
		//readdir(dir);
		//readdir(dir);

		while( ( ent = readdir(dir) ) != NULL ) //get file name
		{

			//if(S_ISDIR(st.st_mode))
			//(directory.c_str() + ent->d_name)

			if(directory + ent->d_name != directory + "." && directory + ent->d_name != directory + "..")
				files_.push_back( directory + ent->d_name );
			else {
				//cout << "Filelister -- " << ent->d_name << endl;
				cout <<  " Remove directories " << directory + ent->d_name << endl;

			}
		}

		closedir(dir); //close directory
	}
	else
	{
		cerr << "ERROR: Impossible to open directory" << endl;
		exit(EXIT_FAILURE);
	}
}

/****************************
 *     GETTERS & SETTERS    *
 ****************************/

vector<string> FileLister::Files()
{
	return files_;
}

/****************************
 *         OTHER            *
 ****************************/

string FileLister::ToString(){
	stringstream out( stringstream::in | stringstream::out );
	unsigned int i;
	for( i=0; i < files_.size(); ++i )
		out << files_[i] << " " << endl;

	out << "numero de arquivos lidos " << i << endl;
	return out.str();
}
