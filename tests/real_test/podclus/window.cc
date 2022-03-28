#include "window.hh"

/****************************
 *        CONSTS            *
 ****************************/
const double Window::kBlankValue = numeric_limits<double>::max();

/****************************
 * CONSTRUCTORS/DESTRUCTORS *
 ****************************/

/**
 * @brief Initializes constants.
 *
 * @param size: value of `kSize`
 */
Window::Window( unsigned int size ) : kSize( size ) {
//	cout << "windows size " << kSize << endl;
}

/****************************
 *     GETTERS & SETTERS    *
 ****************************/

/****************************
 *         OTHER            *
 ****************************/

/**
 * @brief Checks if the window is filled with data. 
 *
 * @details Checks if the window has at least kSize elements.
 *
 * @retvalue TRUE: The window is filled
 * @retvalue FALSE: The window is not filled
 */
bool Window::IsFilled()
{
	if ( size() >= kSize )
		return true;
	return false;
}

/**
 * @brief Inserts a new value in the window.
 *
 * @details If the windows is filled, pops the oldest value and then adds the new value.
 */
void Window::Insert( double v)
{
	if( IsFilled() )
		pop_back();//pop_front();
	push_front(v);//push_back(v);
	Compute();
}

/**
 * @brief Inserts a "blank" value in the window.
 *
 * @details In case there is no data, inserts a "blank" value in the window, actually it only removes the oldest window value.
 *
 */
void Window::InsertBlank()
{
	Insert( kBlankValue );
}


/**
 * @brief Computes statistics from the current elements.
 *
 * @details Erases the old synopsis, and then build another one using all current elements.
 */
void Window::Compute()
{
	Clear();

	for ( unsigned int i = 0; i < size(); ++i )
		if( at(i) != kBlankValue ) {
			Add( at(i) );
			//cout << at(i) << "   " ;
		}
		//else {
		//	cout << at(i) << " Blank" << endl;
		//}
}

/**
 * @brief Multiplies the window by a factor.
 *
 * @details Iterates the window and multiply each position by a factor.
 *
 * @param factor: factor to be multiplied to the window
 */
void Window::Multiply( double factor )
{
	for( unsigned int i=0; i < size(); ++i )
	{
		if ( at(i) != kBlankValue )
			at(i) *= factor;
	}
}
