/*
	Description: It is the set base for the isaac cipher.
				Based off the user Disch's (2011) algorithm
*/

#pragma once
#include <time.h>
#include "isaac.h"
#include "lagged.h"

//TYPE DEF//
typedef unsigned int u32;
typedef unsigned int u8;

//RNGBASE INITIALIZATION//
class RNGBase
{
public:
	//Deconstructor
	virtual ~RNGBase() {}

	//intialize/define functions
	virtual u32 Get() = 0;

	//Calls Get()
	inline u32 operator () () { return Get(); }

	//Reseeding//
	inline void TimeSeed() { Reseed(static_cast<u32>(time(0))); } //returh time after january 1 1970 UTC
	virtual void Reseed(u32 seed) = 0;   // to be supplied by the RNG

};	//RNGBase

class GMSimple32RNG : public RNGBase
{
public:
	//Function definition
	//Runs as soon object is called
	GMSimple32RNG(bool seed = true) { if (seed) TimeSeed(); }

	// takes the values stored in state and returns it after multiplying by 69069 and adding 362437 to the product.
	virtual u32 Get()
	{
		return state = (69069 * state) + 362437;
	}

	//Sets state = to the seed proposed by the programmer
	virtual void Reseed(u32 seed)
	{
		std::cout << seed << std::endl;
		state = seed;
	}

private:
	u32 state;

};	//GMSimple32RNG

extern GMSimple32RNG Rand;