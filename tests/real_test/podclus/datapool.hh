#ifndef DATA_POOL_H
#define DATA_POOL_H

#include<iostream>
#include<fstream>
#include<string>
#include "boost/tokenizer.hpp"

using namespace std;
using namespace boost;

/**
 * @class Datapool datapool.hh
 *
 * @brief Class will access a csv file and create a pool of data.
 *
 * @author Matheus E. Ramos
 * @version Revision 0.3
 * @date 2014 October 15 
 */
class DataPool
{
	private:
		const unsigned int kColumnIndex; //< column index of the value to be retrieved
		ifstream input_; //< input file

	public:
		static const int kNotValidValue; //< value of the exception of a not valid value

		DataPool(string, unsigned int);

		double GetNew() throw(int);
		string GetOptional();
		bool Empty();

};

#endif
