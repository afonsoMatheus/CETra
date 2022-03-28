#include "datapool.hh"


/****************************
 *        CONSTS            *
 ****************************/
const int DataPool::kNotValidValue = -505;

/****************************
 * CONSTRUCTORS/DESTRUCTORS *
 ****************************/

/**
 * @brief Initializes constants and opens input file.
 *
 * @param file_name: input file name
 * @param column_index: column index of the value in the csv file
 */
DataPool::DataPool( string file_name, unsigned int column_index) : kColumnIndex(column_index) {
	input_.open( file_name );
	
	if ( !input_.is_open() && !input_.good() )
	{
		cerr << "Input file stream not good" << endl;
		exit(EXIT_FAILURE);
	}
	
	//take header off of the play
	string temp;
	getline( input_, temp );
	//cout << "Linha lida header  " << temp.c_str() << endl;

}

/****************************
 *     GETTERS & SETTERS    *
 ****************************/

/****************************
 *         OTHER            *
 ****************************/

/**
 * @brief Returns one value from the data pool.
 *
 * @details Reads a line of the input file, get the column value of the kColumnIndex column, if it is valid convert to double and returns, otherwise throw an exception.
 *
 * @throws kNotValidValue: if the value is absent in the column
 *
 * @return file value converted to double
 */ 
double DataPool::GetNew() throw(int)
{
	double v = 0.0;
	string line;
	vector<string> vec;
	
	getline( input_, line );
	//cout << "Linha lida  " << line.c_str() << endl;

	if ( line.empty() ) {
		throw kNotValidValue;
		cout << "linha vazia" << endl;
	}

	if (kColumnIndex > 0) {
		//tokenize csv line and assign to vector
		tokenizer< escaped_list_separator<char> > tok(line);
		vec.assign( tok.begin(), tok.end() );

		if ( vec[kColumnIndex].length() == 0 ) //exception if there's no value available
				throw kNotValidValue;

		v = atof( vec[kColumnIndex].c_str() ); //convert value to double

	} else {
		v = atof( line.c_str() ); //convert value to double
	}

	//optional_value_ = vec[kOptionalIndex];
	//cout << "Date: " << vec[1] << endl;
	

	return v;
}

/**
 * @brief Returns if the data pool is empty.
 *
 * @details Checks if the input file is at the end.
 */
bool DataPool::Empty()
{
	if ( input_.eof() )
		return true;

	return false;
}
