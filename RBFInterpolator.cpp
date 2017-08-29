#include "RBFInterpolator.h"


#include "jama_eig.h"

#include "MathFunction.h"

RBFInterpolator::RBFInterpolator()
{
}


RBFInterpolator::~RBFInterpolator()
{
	if (_pDistance) delete _pDistance;
	if (_pDistanceR) delete _pDistanceR;
	if (_pF) delete _pF;
	if (_pW) delete _pW;
}


void RBFInterpolator::Build(const std::vector<DPoint3>& points, double (*pFunPhi)(double, double), double r) {
	_dbR = r;
	_pFunPhi = pFunPhi;
	// 1.calculation
	int nLen = points.size();
	_pDistance= new Array2D<double>(nLen, nLen);		// distance matrix
	_pDistanceR = new Array2D<double>(nLen, nLen);		// reverse of distance matrix
	_pF = new Array2D<double>(nLen, 1);					// array of f (kernel function)
	_pW = new Array2D<double>(nLen, 1);					// array of w (weight)
																	// initialize the matrix and its reverse
	for (size_t i = 0; i < nLen; i++)
	{
		_points.push_back(points[i]);
		for (size_t j = 0; j < nLen; j++)
		{
			double dbX = points[i].x - points[j].x;
			double dbY = points[i].y - points[j].y;
			(*_pDistance)[i][j] = (*pFunPhi)(sqrt(dbX*dbX + dbY*dbY),r);
		}
	}
	GetMatrixInverse_2(*_pDistance, nLen, *_pDistanceR);
	// initialize the vector of f
	for (size_t i = 0; i < nLen; i++)
	{
		(*_pF)[i][0] = points[i].z;
	}
	// calculate vector of w
	multiply(*_pDistanceR, *_pF, *_pW);
}

double RBFInterpolator::Calculate(double x, double y) {
	int nLen = _points.size();
	double z = 0;
	for (size_t k = 0; k < nLen; k++)
	{
		double dbX = x - _points[k].x;
		double dbY = y - _points[k].y;
		z += (*_pW)[k][0] * (*_pFunPhi)(sqrt(dbX*dbX + dbY*dbY),_dbR);
	}
	return z;
}