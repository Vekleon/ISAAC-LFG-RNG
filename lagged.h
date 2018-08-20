/*
	File name: lagged.h
	Description: shuffle the numbers in an array to create a lagged fibonnaci generator
				nothing in this file will help you understand the program/IA itself.
*/

#pragma once
#include <iostream>
#include <vector>
#include <array>

typedef unsigned int u32;
using namespace std;

//CLASS INTIALIZATION//
class LaggedFibonacci
{
private:
	vector <u32> s;
	int j = 6;
	int k = 31;

public:
	//FUNCTION INITIALIZATION
	LaggedFibonacci() {}

	virtual void retrieve(u32 s);
	virtual u32 input(int t);

	void generate();

	virtual ~LaggedFibonacci() {}
	
};

extern LaggedFibonacci lagged;