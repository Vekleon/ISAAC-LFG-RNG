/*
	Description: The Lagged Fibonacci Generator (LFG) shuffles the arrays index around then adds the values in the array in to the main algorithm to add more entropy.
				algorithm based off Toponce's (2015).
*/
#include "lagged.h"

using namespace std;

//FUNCTION DEFINITION
/*
*retrieve shuffle values into fibonnaci array
*/
void LaggedFibonacci::retrieve(u32 gms) {
	s.push_back(gms);
}

/*
*Returns values within the array to add onto final random output
*/
u32 LaggedFibonacci::input(int t) {
	return s[t];
}

//Shuffle the numbers in the array to simulate the Lagged Fibonnaci Generator.
void LaggedFibonacci::generate() {

	int change;
	for (int i = 0; i < 100; i ++)
	{
		if (i == 0){
			change = (s[j - 1] + s[k - 1]); //this line is the mix up
		}
		else if (i < 99) // set all array values 1 index down.
		{
			s[i] = s[i + 1];
		}
		else
		{
			s[i] = change;//sets the last value in the array to change
		}
	}
}