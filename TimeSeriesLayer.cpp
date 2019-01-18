#include "TimeSeriesLayer.h"

#include "tnt_array1d.h"
#include "tnt_array2d.h"

#include "jama_eig.h"

#include <QGLWidget>
#include <gl/GLU.h>
#include <MathTypes.hpp>
#include "RBFInterpolator.h"
#include "Interpolater1D.h"

#include<MyPCA.h>


TimeSeriesLayer::TimeSeriesLayer() :_dbLeft(-2), _dbRight(2)
{
	// 1.generate the original sequence
	generateSequence();
	return;
	// 2.do interpolation	
	doRBF();
	doLagrangian();
	doKDE();
	doShepards();


	// enable blending
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
}

void TimeSeriesLayer::doRBF() {
	int nLen = _sequence.size();
	// 1.calculation
	Array2D<double> disMatrix = Array2D<double>(nLen, nLen);		// distance matrix
	Array2D<double> disMatrix_r = Array2D<double>(nLen, nLen);		// reverse of distance matrix
	Array2D<double> f = Array2D<double>(nLen, 1);					// array of f
	Array2D<double> w = Array2D<double>(nLen, 1);					// array of w

																	// calculate distance matrix
	for (size_t i = 0; i < nLen; i++)
	{
		for (size_t j = 0; j < nLen; j++)
		{
			disMatrix[i][j] = funPhi(abs(_sequence[i].x - _sequence[j].x), 1);
		}
	}
	for (size_t i = 0; i < nLen; i++)
	{
		f[i][0] = _sequence[i].y;
	}
	PCA::GetMatrixInverse_2(disMatrix, nLen, disMatrix_r);

	PCA::multiply(disMatrix_r, f, w);

	// 2.generate result
	int nResultLen = 101;
	double dbStep = (_dbRight - _dbLeft) / (nResultLen - 1);
	for (size_t i = 0; i < nResultLen; i++)
	{
		double x = _dbLeft + dbStep*i;
		double y = 0;
		for (size_t j = 0; j < nLen; j++)
		{
			y += w[j][0] * funPhi(abs(x - _sequence[j].x), 1);
		}
		_sequenceResultRBF.push_back(DPoint3(x, y, 0));
	}
}

const double c_dbK = 1.0 / sqrt(PI2d);
inline double KernelFun(double para) {
	return c_dbK*exp(-para*para / 2.0);
}

void TimeSeriesLayer::doKDE() {
	double dbStep = (_dbRight - _dbLeft) / (_nResultLen - 1);
	double dbB = 1.0 / _dbH;
	for (size_t i = 0; i < _nResultLen; i++)
	{
		double x = _dbLeft + dbStep*i;
		double y = 0;
		// the base, as the number of points
		double dbBase = 0.0;
		for (DPoint3 pt : _sequence) {
			dbBase += pt.y;
			y += pt.y*KernelFun((x - pt.x) / dbB) / dbB;
		}
		y /= dbBase;

		_sequenceResultKDE.push_back(DPoint3(x, y, 0));
	}
}

void TimeSeriesLayer::doShepards() {
	Interpolater1D* pInterpolater = Interpolater1D::CreateInterpolater(Interpolater1D::IT_Shepards, _sequence, _dbH);

	double dbStep = (_dbRight - _dbLeft) / (_nResultLen - 1);
	double dbB = 1.0 / _dbH;
	for (size_t i = 0; i < _nResultLen; i++)
	{
		double x = _dbLeft + dbStep*i;
		double y = pInterpolater->Interpolate(x);

		_sequenceResultShepards.push_back(DPoint3(x, y, 0));
	}

	delete pInterpolater;
}

void TimeSeriesLayer::doLagrangian() {
	int nLen = _sequence.size();
	// 2.generate result_nResultLen
	double dbStep = (_dbRight - _dbLeft) / (_nResultLen - 1);
	for (size_t i = 0; i < _nResultLen; i++)
	{
		double x = _dbLeft + dbStep*i;
		double y = 0;
		for (DPoint3& ptj : _sequence)
		{
			double l = 1;
			for (DPoint3& ptk : _sequence)
			{
				if (!(ptj == ptk))
				{
					l *= (x - ptk.x) / (ptj.x - ptk.x);
				}
			}
			y += ptj.y*l;

		}
		_sequenceResultLagrangian.push_back(DPoint3(x, y, 0));
	}
}

TimeSeriesLayer::~TimeSeriesLayer()
{
}

void TimeSeriesLayer::Draw() {
	glPushMatrix();
	double dbBiasX = -_nLen*_dbStep / 2.0;
	double dbBiasY = 0;
	glScaled(.3, .3, 1);
	glTranslated(dbBiasX,dbBiasY, 0);
	// draw points
	glPointSize(_pSetting->_dbPointSize);

	bool bShowPoints = true;
	bool bShowLines = false;
	if (bShowPoints) {
		glColor4f(0, 0, 1, 1);
		glBegin(GL_POINTS);
		for (DPoint3 pt : _sequence)
		{
			glVertex3d(pt.x, pt.y, pt.z);
		}
		glEnd();
	}

	// draw polyline
	if (bShowLines)
	{
		glColor4f(0, 0, 1, .5);
		int nSeqLen = _sequence.size();
		glBegin(GL_LINE_STRIP);
		for (size_t i = 0; i < nSeqLen; i++)
		{
			glVertex3d(_sequence[i].x, _sequence[i].y, _sequence[i].z);
		}
		glEnd();

	}
	glPopMatrix();
}

void TimeSeriesLayer::generateSequence(int nLen, int nPeriod) {
//	generateSeq1();
	generateSeq2();
}
void TimeSeriesLayer::generateSeq1() {
	for (size_t i = 0; i < _nLen; i++)
	{
		double x = i*_dbStep;
		double y = sin(x)*_dbRange;
		_sequence.push_back(DPoint3(x, y, 0));
	}
	vector<double> vecNormal = generateNormalSequence(_nLen, 0, 1);
	vector<double> vecRandom1 = generateRandomSequence(_nLen, 1.0);
	vector<double> vecRandom2 = generateRandomSequence(_nLen, 1.0);

	double dbBeta = .1;
	double dbDelta = 2.0;
	for (size_t i = 0; i < _nLen; i++)
	{
		_sequence[i].y += dbBeta*vecNormal[i];
		_sequence[i].y += vecRandom1[i]>.8 ? dbDelta*(vecRandom2[i] - .5) : 0;
	}
}

void TimeSeriesLayer::generateSeq2() {
	_nLen = 1000*PId;
	_dbStep = .003;
	for (size_t i = 0; i < _nLen; i++)
	{
		double x = i*_dbStep;
		double y = sin(i)*_dbRange;
		_sequence.push_back(DPoint3(x, y, 0));
	}
}

void TimeSeriesLayer::Reset(int nLen, int nPeriod) {
	_sequence.clear();
	_sequenceResultRBF.clear();
	_sequenceResultLagrangian.clear();
	_sequenceResultKDE.clear();
	_sequenceResultShepards.clear();

	generateSequence(nLen, nPeriod);

	doRBF();
	doLagrangian();
	_dbH = nPeriod;
	doKDE();
	doShepards();
}