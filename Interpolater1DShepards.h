#pragma once
#include <MathTypes.hpp>
#include <vector>

#include "Interpolater1D.h"

class Interpolater1DShepards:public Interpolater1D
{
public:
	Interpolater1DShepards(std::vector<DPoint3> vecSamples,double dbP);
	virtual ~Interpolater1DShepards();
public:
	virtual double Interpolate(double x);
};

