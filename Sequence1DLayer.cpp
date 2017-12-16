#include "Sequence1DLayer.h"

#include "tnt_array1d.h"
#include "tnt_array2d.h"

#include "jama_eig.h"

#include <QGLWidget>
#include <gl/GLU.h>
#include "MathFunction.h"
#include "RBFInterpolator.h"
#include "Interpolater1D.h"

void forwardDFT(const std::vector<DPoint3> s, int nLen, std::vector<double>& a, std::vector<double>& b)
{
	// note: this code is not optimised at all, written for clarity not speed.
	for (int k = 0; k <= nLen / 2; ++k) {
		double newA = 0;
		double newB = 0;
		for (int x = 0; x < nLen; ++x) {
			newA += s[x].y * cos(2 * PId / nLen * k * x);
			newB += s[x].y * sin(2 * PId / nLen * k * x);
		}
		// normalization
		newA *= (k == 0 || k == nLen / 2) ? 1. / nLen : 2. / nLen;
		newB *= 2. / nLen;
		a.push_back(newA);
		b.push_back(newB);
	}
}

const bool c_bFFT = false;

Sequence1DLayer::Sequence1DLayer():_dbLeft(-2),_dbRight(2)
{
	// 1.generate the original sequence
	generateSequence();
//	return;
	// 2.do interpolation	
	doRBF();		
	doLagrangian();
	doKDE();
	doShepards();
}

void Sequence1DLayer::doRBF() {
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
			disMatrix[i][j] = funPhi(abs(_sequence[i].x - _sequence[j].x),1);
		}
	}
	for (size_t i = 0; i < nLen; i++)
	{
		f[i][0] = _sequence[i].y;
	}
	GetMatrixInverse_2(disMatrix, nLen, disMatrix_r);

	multiply(disMatrix_r, f, w);

	// 2.generate result
	int nResultLen = 101;
	double dbStep = (_dbRight-_dbLeft) / (nResultLen-1);
	for (size_t i = 0; i < nResultLen; i++)
	{
		double x = _dbLeft + dbStep*i;
		double y = 0;
		for (size_t j = 0; j < nLen; j++)
		{
			y += w[j][0] * funPhi(abs(x - _sequence[j].x),1);
		}
		_sequenceResultRBF.push_back(DPoint3(x, y, 0));
	}
}

const double c_dbK = 1.0 / sqrt(PI2d);
inline double KernelFun(double para) {
	return c_dbK*exp(-para*para / 2.0);
}

void Sequence1DLayer::doKDE() {
	double dbStep = (_dbRight - _dbLeft) / (_nResultLen - 1);
	double dbB = 1.0/_dbH;
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

void Sequence1DLayer::doShepards() {
	Interpolater1D* pInterpolater=Interpolater1D::CreateInterpolater(Interpolater1D::IT_Shepards,_sequence, _dbH);

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

void Sequence1DLayer::doLagrangian() {
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
				if (! (ptj == ptk)) 
				{
					l *= (x - ptk.x) / (ptj.x - ptk.x);
				}
			}
			y += ptj.y*l;

		}
		_sequenceResultLagrangian.push_back(DPoint3(x, y, 0));
	}
}

Sequence1DLayer::~Sequence1DLayer()
{
}

void Sequence1DLayer::Draw() {
	// draw points
	glPointSize(_pSetting->_dbPointSize);

	glBegin(GL_POINTS);
	for (DPoint3 pt : _sequence)
	{
		glVertex3d(pt.x, pt.y, pt.z);
	}
	glEnd();

	/*
	// draw polyline
	glColor3f(0, 1, 1);
	int nSeqLen = _sequence.size();
	glBegin(GL_LINE_STRIP);
	for (size_t i = 0; i < nSeqLen; i++)
	{
		glVertex3d(_sequence[i].x, _sequence[i].y, _sequence[i].z);
	}
	glEnd();
	*/

	// draw RBF results
	glColor3f(1, 0, 0);
	int nSeqResultLen = _sequenceResultRBF.size();
	glBegin(GL_LINE_STRIP);
	for (size_t i = 0; i < nSeqResultLen; i++)
	{
		glVertex3d(_sequenceResultRBF[i].x, _sequenceResultRBF[i].y, _sequenceResultRBF[i].z);
	}
	glEnd();

	// draw Lagrangian results
	glColor3f(0, 1, 0);
	nSeqResultLen = _sequenceResultLagrangian.size();
	glBegin(GL_LINE_STRIP);
	for (size_t i = 0; i < nSeqResultLen; i++)
	{
		glVertex3d(_sequenceResultLagrangian[i].x, _sequenceResultLagrangian[i].y, _sequenceResultLagrangian[i].z);
	}
	glEnd();

	// draw KDE results
	glColor3f(0, 0, 1);
	nSeqResultLen = _sequenceResultKDE.size();
	glBegin(GL_LINE_STRIP);
	for (size_t i = 0; i < nSeqResultLen; i++)
	{
		glVertex3d(_sequenceResultKDE[i].x, _sequenceResultKDE[i].y, _sequenceResultKDE[i].z);

	}
	glEnd();



	// draw Shepards results
	glColor3f(1, 0, 1);
	glBegin(GL_LINE_STRIP);
	for (DPoint3 pt : _sequenceResultShepards)
	{
		glVertex3d(pt.x, pt.y, 0);
	}
	glEnd();
}

void Sequence1DLayer::generateSequence(int nLen, int nPeriod) {
	if (c_bFFT) {
		nLen *= 10;
		// generate a sine wave
		double dbScope = 4;
		double dbLeft = -2;
		double dbStep = dbScope / nLen;

		for (size_t i = 0; i <= nLen; i++)
		{
			double x = dbLeft + i*dbStep;
			//		double y = sin(x*PIf*nPeriod);
			double scaledX = i / 100.0;// x*PIf*nPeriod;
			double y = sin(scaledX) + cos(10 * scaledX) + 0.5 * cos(40 * scaledX);
			y /= 10;
			_sequence.push_back(DPoint3(x, y, 0));
		}
		std::vector<double> a, b;
		forwardDFT(_sequence, nLen, a, b);
	}
	else
	{
		// 0.generation
		DPoint3 seq[10] = {
			DPoint3(-2, -.2, 0)
			,DPoint3(-1.6, -0.6, 0)
			,DPoint3(-1.2, 0, 0)
			,DPoint3(-0.8, .3, 0)
			,DPoint3(-0.1, .5, 0)
			,DPoint3(-0.0, .7, 0)
			,DPoint3(0.4, .9, 0)
			,DPoint3(0.6, .5, 0)
			,DPoint3(1.2, .3, 0)
			,DPoint3(1.6, .0, 0)
		};
		int nLen = 10;
		for (size_t i = 0; i < nLen; i++)
		{
			//		double x = i;
			//		double y = MyRandom()*4-2;
			//		_sequence.push_back(DPoint3(x, y, 0));
			_sequence.push_back(seq[i]);
			//		_sequenceResult.push_back(DPoint3(seq[i].x, -seq[i].y,0));
		}
	}

}

void Sequence1DLayer::Reset(int nLen, int nPeriod) {
	_sequence.clear();
	_sequenceResultRBF.clear();
	_sequenceResultLagrangian.clear();
	_sequenceResultKDE.clear();
	_sequenceResultShepards.clear();

	generateSequence(nLen, nPeriod);
	if (!c_bFFT) {
		doRBF();
		doLagrangian();
		_dbH = nPeriod;
		doKDE();
		doShepards();
	}

}