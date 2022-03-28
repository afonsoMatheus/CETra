#ifndef WINDOW_H
#define WINDOW_H

#include<iostream>
#include<deque>
#include<limits>

#include "estatisticas.hh"

using namespace std;

/**
 * @class Window window.hh
 *
 * @brief Class will keep a window of double values of `kSize` and keep statistics updated.
 *
 * @notes Only the method `Insert` should be used to manipulate data. Inserting data in another way could result
 * in malformed data.
 *
 * @author Matheus E. Ramos
 * @version Revision 0.3
 * @date September 19 2014
 */
class Window: public deque<double>, public Estatisticas
{
	private:
		const unsigned int kSize; /**< Size of the window */
		static const double kBlankValue; /**< Value that will be considered as nothing */

		void Compute();

	public:
		Window( unsigned int );

		bool IsFilled();
		void Insert(double);
		void InsertBlank();
		void Multiply(double);
};
#endif
