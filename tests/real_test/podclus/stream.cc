#include "stream.hh"

/****************************
 *        CONSTS            *
 ****************************/

/****************************
 * CONSTRUCTORS/DESTRUCTORS *
 ****************************/

Stream::Stream( string file_name, unsigned int window_size, unsigned int column_index)
{
    pool_ = new DataPool( file_name, column_index);
	id_ = atoi( file_name.substr( file_name.find_last_of('/') + 1 ).c_str() ); 
	
	window_ = new Window( window_size );
	//if (id_ == 163)
	//	cout << "arquivo " << file_name << "  id = " << id_ << endl;
	while( !window_->IsFilled() )
	{
		Next();
	}
}

Stream::~Stream()
{
	delete window_;
	delete pool_;
}

/****************************
 *     GETTERS & SETTERS    *
 ****************************/
long int Stream::Id()
{
	return id_;
}

Estatisticas Stream::Synopsis()
{
	return *window_;
}

Window Stream::W()
{
	return *window_;
}

bool Stream::HasNew()
{
	return !pool_->Empty();
}

/****************************
 *         OTHER            *
 ****************************/

/**
 * @brief Multiplies the window by a factor.
 *
 * @details Iterates the window and multiply each position by a factor.
 *
 * @param factor: factor to be multiplied to the window
 */
void Stream::Multiply( double factor )
{
	window_->Multiply( factor );
	//for( unsigned int i=0; i < window_->size(); ++i )
		//window_->at(i) *= factor;
}

/**
 * @brief Updates window with new data.
 *
 * @details Takes off the oldest element and puts a new one.
 */
void Stream::Next()
{
	float f = 0.0;
	try
	{
		f = pool_->GetNew();
		//cout << f << "  ";
		window_->Insert( f );

	}
	catch( ... )
	{
		//cout << "insert blank " << id_ << endl;
		window_->InsertBlank();
	}
}

/**
 * @brief Shifts n elements from the windows.
 *
 * @details Removes the 'shift_size' oldest elements from the front and adds `shift_size` elements at the back
 *
 * @param size: size of the shift
 */
void Stream::Shift(unsigned int shift_size)
{
	for( unsigned int i=0; i < shift_size; ++i )
	{
		if( pool_->Empty() ) break;
		Next();
	}

	//cout << "CURRENT STREAM AFTER SHIFT: " << ToString() << endl;
}


string Stream::ToString()
{
	stringstream ss( stringstream::in | stringstream::out );

	ss << "STREAM " << Id() << "," << window_->ToString();

	return ss.str();
}
