#pragma once
#include <vector>
#include <MathTypes.hpp>
#include "tnt_array2d.h"
/*
	class to represent a confidence ellipse
	mingdong
	2017/07/31
*/
class ConfidenceEllipse
{
public:
	ConfidenceEllipse(std::vector<DPoint3> vecPts);
	ConfidenceEllipse(std::vector<LabeledPoint> vecPts);
	virtual ~ConfidenceEllipse();
	// calculate alpha value of a given point
	double CalculateAlpha(DPoint3 pt);
protected:
	int _nRow;
	int _nCol=2;
	double _dbMeanX;
	double _dbMeanY;
	TNT::Array2D<double>* _pD;
	TNT::Array2D<double>* _pCM;
	TNT::Array2D<double>* _pCMR;
};

