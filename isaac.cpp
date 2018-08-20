/*
	CPP name: isaac.cpp
	Description: define the intialized functions of isaac.h
	Based off user Disch (2011)'s algorithm
*/


#include <iostream>
#include "isaac.h"
#include "lagged.h"

namespace dshlib
{
	//RESEED//
	/*
	*collects values into the array seed[] based off seconds after january 1 1970 UTC.
	*/
	void IsaacRNG::Reseed(u32 seed)
	{
		//Run GMSimple32RNG to collect seeds for isaac
		GMSimple32RNG r(false);
		r.Reseed(seed);	//This is time related. It returns a value of the amount of seconds after the date 1 January 1970.
		u32 seeds[100]; //intialize the number of seeds needed

		int i;
		//sets value given by GMSimple32RNG into the seed array (Bases off time from 1 January 1970 in seconds and multiplies it by 69069 and adds 362437 to it.
		//the variable u32 can only go up to the value 4294967295 before it loops back to 0 and adds the remaining values.
		//Each value of seeds array is a u32 value
		for (i = 0; i < 100; ++i)
		{
			seeds[i] = r();
		}

		//Call the function ReseedFull using the values of seed
		ReseedFull(seeds);
	}

	//MIX//
	/*
	*mixes up the values of 8 variables by taking the power of them, bitshifting and adding.
	*/
	static inline void mix(u32& a, u32& b, u32& c, u32& d, u32& e, u32& f, u32& g, u32& h)
	{
		a ^= b << 11;   d += a;   b += c;	//^= means to take the current value of that variable and make it = to itself and take the power of the variable on the right
		b ^= c >> 2;    e += b;   c += d;	// << and >> is defined within the paper work of the IA
		c ^= d << 8;    f += c;   d += e;	
		d ^= e << 16;   g += d;   e += f;
		e ^= f << 10;   h += e;   f += g;
		f ^= g >> 4;    a += f;   g += h;
		g ^= h << 8;    b += g;   h += a;
		h ^= a >> 9;    c += h;   a += b;
	}

	//RESEED FULL//
	/*
	* intialize variables a - h which serve the purpose of mixing up the array mm[]
	*/
	void IsaacRNG::ReseedFull(const u32 seeds[100])
	{
		//Variable intializiation
		aa = 0;
		bb = 0;
		cc = 0;
		step = 0;

		//Variables used to conduct the above and below the median runs test
		ac = 0;	//above consecutively
		bc = 0; //below consecutively
		prerun = 0;
		runs = 0;

		//used for the reverse arrangements test. The A value
		reverse = 0;

		//Set values for counter which is used to keep track of the amount of times each value between 1 - 25 appears, as well as to find the mean
		for (int k = 0; k < 25; k++)
		{
			counter[k][0] = k+1;
			counter[k][1] = 0;
		}

		//Set a - h as the golden ratio which will be used to mix the array values of mm[]
		static const u32 GOLDENRATIO = 0x9E3779B9; //2654435769

		u32 a, b, c, d, e, f, g, h;
		a = b = c = d = e = f = g = h = GOLDENRATIO;

		int i;
		//mixes a - h 4 times using the mix() function stated above
		for (i = 0; i < 4; ++i)
			mix(a, b, c, d, e, f, g, h);

		//shuffle up a-h values even mroe than acqurie mm[] values
		for (i = 0; i < 100; i += 8)
		{
			a += seeds[i];       b += seeds[i + 1];   c += seeds[i + 2];   d += seeds[i + 3];
			e += seeds[i + 4];   f += seeds[i + 5];   g += seeds[i + 6];   h += seeds[i + 7];

			mix(a, b, c, d, e, f, g, h);

			mm[i] = a;       mm[i + 1] = b; mm[i + 2] = c; mm[i + 3] = d;
			mm[i + 4] = e;   mm[i + 5] = f; mm[i + 6] = g; mm[i + 7] = h;

		}

		//Shuffle up mm again to ensure randomness
		for (i = 0; i < 100; i += 8)
		{
			a += seeds[i];       b += seeds[i + 1];   c += seeds[i + 2];   d += seeds[i + 3];
			e += seeds[i + 4];   f += seeds[i + 5];   g += seeds[i + 6];   h += seeds[i + 7];

			mix(a, b, c, d, e, f, g, h);

			mm[i] = a;       mm[i + 1] = b; mm[i + 2] = c; mm[i + 3] = d;
			mm[i + 4] = e;   mm[i + 5] = f; mm[i + 6] = g; mm[i + 7] = h;
		}

		//Retrieve the values the number of values, currently set to retrieve 100.
		for (i = 0; i < 100; ++i)
			xi[i] = Get();

		//Declare variables to find the number of times each value appeared, as well as the probability of their appearence and the mean of the whole set.
		float mean = 0;
		int numberCount = 0;
		for (int k = 0; k < 25; k++)
		{
			
			std::cout << "The value " << counter[k][0] << " appeared " << counter[k][1] << " times out of " << (step ) << ", making the probability " << float(counter[k][1]) / 100 << std::endl;
			mean += counter[k][0] * counter[k][1];
			numberCount += counter[k][1];
			if(k == 24) { mean /= 100; }
		}

		/*
		*this portion of the algorithm conducts the
		*/
		for (int u = 0; u < 99; u++)
		{
			for (int v = u+1; v < 100; v++)
			{
				if (xi[u] > xi[v])
				{
					reverse++;
				}
			}
		}

		//Display results on screen.
		std::cout << "The mean is " << mean << " and the numbers that have appeared is " << numberCount << std::endl;
		std::cout << "The number of runs is " << runs << " and ac " << ac << " and bc " << bc << std::endl;
		std::cout << "The A value is " << reverse << std::endl;
	}

	//GET//
	/*
	* Retreives the random value using the array mm[]
	*/
	u32 IsaacRNG::Get()
	{

		if (!step)//checks if step is 0
		{
			cc += 1;
			bb += cc;
		}

		switch (step & 3) //Checks how many bits step and 3 have the same
		{
		case 0: aa ^= (aa << 13); break; //no bits are the same
		case 1: aa ^= (aa >> 6); break;  //0001 match
		case 2: aa ^= (aa << 2); break;  //0010 match
		case 3: aa ^= (aa >> 16); break; //0011 match
		}

		//Variable intialization//
		u32 x, y;
		LaggedFibonacci run;

		//sets values for the array in the Fibonnaci generator	
		for (int j = 0; j < 100; j++)
		{
				run.retrieve(mm[j]);
				//std::cout << run.input(j) << std::endl;
		}
		//Shuffles values in the Fibonnaci array
		run.generate();

		//Calculate value
		x = mm[step];
		aa = mm[step ^ 0x80] + aa; //0x80 = 128
		mm[step] = y = mm[(x >> 2) & 0xFF] + aa + bb; 
		bb = ((mm[(y >> 10) & 0xFF] + x) + run.input(step))%25 + 1; //0xFF = 255, %100 +1 to return a value between 1 - 100

		//Return value and set up for the next iteration
		//std::cout << bb << std::endl;
		++step;

		//-----------------------------------------------------------------------------------------------------------------------------------//
		/*
		*This portion of the algorithm checks conducts the Test of Runs Above and Below the Median
		*/
		for (int k = 0; k < 25; k++)
		{
			if (counter[k][0] == bb)
			{
				counter[k][1] ++;
			}
		}

		if (bb < 13) { prerun = -1; } //if its below the median
		else if (bb > 13) { prerun = 1; } //if its above the median
		else if (bb == 13) //if it is the median
		{
			if (consec >= 2)
			{
				runs++;
			}
			prerun = 0;
			consec = 0;
		}

		///////////////if a number is above or below the median CONSEQUTIVELY/////////////////
		if (consec >= 2 || consec <= -2)
		{
			//if the trend continues keep running (above median)
			if ((consec >= 2) && (prerun == 1))
			{
				consec++;
				prerun = 0;
				ac++;
			}
			//if the trend stops reset the values and increase the amount of runs
			else if ((consec >= 2) && (prerun == -1))
			{
				consec = -1;
				prerun = 0;
				runs++;
				//cout << "yay above" << endl;
			}

			//if the trend continues: keep running (below median)
			if ((consec <= -2) && (prerun == -1))
			{
				consec--;
				prerun = 0;
				bc++;
			}
			//if the trend stops
			else if ((consec <= -2) && (prerun == 1))
			{
				consec = 1;
				prerun = 0;
				runs++;
				//cout << "yay below" << endl;
			}
		}	//run worthy
		//////////////////if a number is above or below the median, 2nd runthrough////////////////////////
		else if (consec == 1 || consec == -1)
		{
			//number above the median consequtively 
			if ((consec == 1) && (prerun == 1))
			{
				consec++;
				prerun = 0;
			}
			//not above the median
			else if ((consec == 1) && (prerun == -1))
			{
				consec = -1;
				prerun = 0;
				runs++;
			}

			//number below the median consecutively
			if ((consec == -1) && (prerun == -1))
			{
				consec--;
				prerun = 0;
			}
			//not below the median
			else if ((consec == -1) && (prerun == 1))
			{
				consec = 1;
				prerun = 0;
				runs++;
			}
		}	//consecutive check
		/////////////////First number of the cycle//////////////////////////
		else
		{
			if (prerun == 1)
			{
				consec++;
			}
			else if (prerun == -1)
			{
				consec--;
			}
		}	//first cycle

		return bb;
	}	//Get()
}	//namespace dshlib