#ifndef STREAM_H
#define STREAM_H

#include<deque>
#include<iostream>
#include<string>

#include "datapool.hh"
#include "estatisticas.hh"
#include "window.hh"

using namespace std;

/**
 * A stream corresponds to one source of data, a sensor for example.
 */
class Stream{
	private:
		long int id_;
		DataPool *pool_;
		Window *window_;

	public:
		Stream(string, unsigned int, unsigned int);
		~Stream();

		long int Id();
		Estatisticas Synopsis();
		Window W();
		bool HasNew();

		void Multiply(double);
		void Next();
		void Shift(unsigned int);

		string ToString();

};

#endif
