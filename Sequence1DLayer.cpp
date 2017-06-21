#include "Sequence1DLayer.h"

#include "tnt_array1d.h"
#include "tnt_array2d.h"

#include "jama_eig.h"

#include <QGLWidget>
#include <gl/GLU.h>
#include "MathFunction.h"



Sequence1DLayer::Sequence1DLayer():_dbLeft(-2),_dbRight(2)
{
	_sequence.clear();
	_sequenceResultRBF.clear();
	_sequenceResultLagrangian.clear();
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

	// RBF
	doRBF();
	doLagrangian();
}

void Sequence1DLayer::doRBF() {
	int nLen = _sequence.size();
	// 1.calculation

	Array2D<double> disMatrix = Array2D<double>(nLen, nLen);	// distance matrix
	Array2D<double> disMatrix_r = Array2D<double>(nLen, nLen);	// reverse of distance matrix
	Array2D<double> f = Array2D<double>(nLen, 1);					// array of f
	Array2D<double> w = Array2D<double>(nLen, 1);					// array of w
	for (size_t i = 0; i < nLen; i++)
	{
		for (size_t j = 0; j < nLen; j++)
		{
			disMatrix[i][j] = funPhi(abs(_sequence[i].x - _sequence[j].x));
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
			y += w[j][0] * funPhi(abs(x - _sequence[j].x));
		}
		_sequenceResultRBF.push_back(DPoint3(x, y, 0));
	}
}

void Sequence1DLayer::doLagrangian() {
	int nLen = _sequence.size();
	// 2.generate result
	int nResultLen = 101;
	double dbStep = (_dbRight - _dbLeft) / (nResultLen - 1);
	for (size_t i = 0; i < nResultLen; i++)
	{
		double x = _dbLeft + dbStep*i;
		double y = 0;
		for (size_t j = 0; j < nLen; j++)
		{
			Point ptj = _sequence[j];
			double l = 1;
			for (size_t k = 0; k < nLen; k++)
			{
				Point ptk = _sequence[k];
				if (k != j) {
					l *= (x-ptk.x) / (ptj.x-ptk.x);
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
	// draw polyline
	glColor3f(0, 1, 1);
	int nSeqLen = _sequence.size();
	glBegin(GL_LINE_STRIP);
	for (size_t i = 0; i < nSeqLen; i++)
	{
		glVertex3d(_sequence[i].x, _sequence[i].y, _sequence[i].z);
	}
	glEnd();
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
	glColor3f(1, 1, 0);
	nSeqResultLen = _sequenceResultLagrangian.size();
	glBegin(GL_LINE_STRIP);
	for (size_t i = 0; i < nSeqResultLen; i++)
	{
		glVertex3d(_sequenceResultLagrangian[i].x, _sequenceResultLagrangian[i].y, _sequenceResultLagrangian[i].z);

	}
	glEnd();
}