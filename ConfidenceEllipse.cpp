#include "ConfidenceEllipse.h"

#include "MathFunction.h"

ConfidenceEllipse::ConfidenceEllipse(std::vector<Point> vecPts):_nRow(vecPts.size()),_nCol(2)
{
	_pD = new Array2D<double>(_nRow, _nCol);
	_pCM = new Array2D<double>(_nCol, _nCol);
	_pCMR = new Array2D<double>(_nCol, _nCol);

	// 0.calculate the mean
	_dbMeanX = 0;
	_dbMeanY = 0;

	for (int i = 0; i < _nRow; i++)
	{
		_dbMeanX += vecPts[i].x;
		_dbMeanY += vecPts[i].y;
	}
	_dbMeanX /= _nRow;
	_dbMeanY /= _nRow;

	// 1.generate intput data
	for (int i = 0; i < _nRow; i++)
	{
		(*_pD)[i][0] = vecPts[i].x - _dbMeanX;
		(*_pD)[i][1] = vecPts[i].y - _dbMeanY;
	}


	// 2.compute covariance matrix
	compute_covariance_matrix(*_pD, *_pCM);

	// 3.compute the reverse matrix of covariance
	GetMatrixInverse(*_pCM, _nCol, *_pCMR);
}

ConfidenceEllipse::ConfidenceEllipse(std::vector<LabeledPoint> vecPts) :_nRow(vecPts.size()), _nCol(2)
{
	_pD = new Array2D<double>(_nRow, _nCol);
	_pCM = new Array2D<double>(_nCol, _nCol);
	_pCMR = new Array2D<double>(_nCol, _nCol);

	// 0.calculate the mean
	_dbMeanX = 0;
	_dbMeanY = 0;

	for (int i = 0; i < _nRow; i++)
	{
		_dbMeanX += vecPts[i]._arrCoord[0];
		_dbMeanY += vecPts[i]._arrCoord[1];
	}
	_dbMeanX /= _nRow;
	_dbMeanY /= _nRow;

	// 1.generate intput data
	for (int i = 0; i < _nRow; i++)
	{
		(*_pD)[i][0] = vecPts[i]._arrCoord[0] - _dbMeanX;
		(*_pD)[i][1] = vecPts[i]._arrCoord[1] - _dbMeanY;
	}


	// 2.compute covariance matrix
	compute_covariance_matrix(*_pD, *_pCM);

	// 3.compute the reverse matrix of covariance
	GetMatrixInverse(*_pCM, _nCol, *_pCMR);
}


ConfidenceEllipse::~ConfidenceEllipse()
{
	delete _pD;
	delete _pCM;
	delete _pCMR;
}


double ConfidenceEllipse::CalculateAlpha(Point pt) {

	Array2D<double> m0(1, 2);
	Array2D<double> m1(2, 1);
	Array2D<double> m(1, 2);
	Array2D<double> mr(1, 1);

	m0[0][0] = m1[0][0] = pt.x - _dbMeanX;
	m0[0][1] = m1[1][0] = pt.y - _dbMeanY;
	multiply(m0, *_pCMR, m);
	multiply(m, m1, mr);
	double alpha = sqrt(mr[0][0]);
	return alpha;
}