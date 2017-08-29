#pragma once
#include <vector>
#include <MathTypes.hpp>
#include "tnt_array2d.h"
/*
	Interpolator use radial basis function 
	Mingdong
	the codes were written according to
	section 2.3. of "Visualizing Time-Specific Hurricane Predictions, with Uncertainty, from Storm Path Ensembles"
	2017/08/14
*/
class RBFInterpolator
{
public:
	RBFInterpolator();
	virtual ~RBFInterpolator();
public:
	// build the interpolator
	void Build(const std::vector<DPoint3>& points,double(*pFunPhi)(double, double),double r=1);
	// calculate the value given coordinate
	double Calculate(double x, double y);
private:
	TNT::Array2D<double>* _pDistance = NULL;	// distance matrix
	TNT::Array2D<double>* _pDistanceR = NULL;	// reverse of distance matrix
	TNT::Array2D<double>* _pF = NULL;			// array of f (kernel function)
	TNT::Array2D<double>* _pW = NULL;			// array of w (weight)
	double(*_pFunPhi)(double,double) = NULL;		// the phi function
	std::vector<DPoint3> _points;				// the points

	double _dbR=1;								// r used in the phi function
};

