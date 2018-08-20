/*
	Header name: isaac.h
	Description: Initiate the features of the IsaacRNG class.
				 this file will not contribute anything to the understanding of the IA
*/

#pragma once
#ifndef __DSHLIB_ISAAC__
#define __DSHLIB_ISAAC__
#include <vector>
#include <iostream>
#include "RNGBase.h"

//TYPE DEF//
typedef unsigned int u32;
typedef unsigned int u8;


namespace dshlib
{
	//CLASS INITIALIZATION
	class IsaacRNG : public RNGBase
	{
	public:
		//Function definition/intializaton
		IsaacRNG(bool seed = true) { if (seed) TimeSeed(); }

		virtual u32 Get();

		virtual void Reseed(u32 seed);
		void ReseedFull(const u32 seeds[100]);

	private:
		//variables for the main generator
		u32 mm[120];
		u32 aa;
		u32 bb;
		u32 cc;
		u8 step;
		int counter[25][2];

		//variables to conduct the runs above and below the median test
		int consec;
		int prerun;
		int runs;
		int ac;
		int bc;

		//variables for the reverse arrangements test
		int reverse;
		int xi[100];
	};	//IsaacRNG

}	//namespace dshlib


#endif