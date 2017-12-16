#pragma once
#include <MathTypes.hpp>
#include <vector>

class Interpolater1D
{
public:
	Interpolater1D(std::vector<DPoint3> vecSamples,double dbP);
	virtual ~Interpolater1D();
public:
	virtual double Interpolate(double x) = 0;
protected:
	std::vector<DPoint3> _vecSamples;
	double _dbP;
public:
	enum Enum_Type
	{
		IT_Shepards,
	};
	static Interpolater1D* CreateInterpolater(Enum_Type type, std::vector<DPoint3> vecSamples, double dbP);
};

