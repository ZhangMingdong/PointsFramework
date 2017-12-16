#pragma once
#include <MathTypes.hpp>
#include <vector>
/*
	used to interpolate 1D sequence
	2017/12/16
*/
class Interpolater1D
{
public:
	Interpolater1D(std::vector<DPoint3> vecSamples,double dbP);
	virtual ~Interpolater1D();
public:
	// do the interpolate
	virtual double Interpolate(double x) = 0;
protected:
	// sample sequence
	std::vector<DPoint3> _vecSamples;
	// parameter
	double _dbP;
public:
	// type of the method
	enum Enum_Type
	{
		IT_Shepards,
	};
	// create new instance
	static Interpolater1D* CreateInterpolater(Enum_Type type, std::vector<DPoint3> vecSamples, double dbP);
};

