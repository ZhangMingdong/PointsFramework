#pragma once
#include <MathTypes.hpp>
#include <vector>

#include "Interpolater1D.h"

/*
	interpolate 1d sequence using shepard's method
*/
class Interpolater1DShepards:public Interpolater1D
{
public:
	Interpolater1DShepards(std::vector<DPoint3> vecSamples,double dbP);
	virtual ~Interpolater1DShepards();
public:
	virtual double Interpolate(double x);
};

