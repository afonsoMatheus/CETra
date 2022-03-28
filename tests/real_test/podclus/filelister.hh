#ifndef FILE_LISTER_H
#define FILE_LISTER_H

#include<iostream>
#include<sstream>
#include<vector>
#include<dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

/**
 * Gets a list of file paths and ids (file name must be a number ).
 */
class FileLister{
	private:
		vector< string > files_; //id and path of the file

	public:
		FileLister(string);
		vector<string> Files();
		string ToString();

};

#endif
