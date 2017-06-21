#include "Sequence1DLayer.h"

#include "tnt_array1d.h"
#include "tnt_array2d.h"

#include "jama_eig.h"

#include <QGLWidget>
#include <gl/GLU.h>
#include "MathFunction.h"



Sequence1DLayer::Sequence1DLayer()
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
	_sequence.clear();
	_sequenceResult.clear();
	int nLen = 10;
	for (size_t i = 0; i < nLen; i++)
	{
		//		double x = i;
		//		double y = MyRandom()*4-2;
		//		_sequence.push_back(DPoint3(x, y, 0));
		_sequence.push_back(seq[i]);
		//		_sequenceResult.push_back(DPoint3(seq[i].x, -seq[i].y,0));
	}
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
			cout << disMatrix[i][j] << "\t";
		}
		cout << endl;
	}
	cout << endl;
	for (size_t i = 0; i < nLen; i++)
	{
		f[i][0] = _sequence[i].y;
	}
	GetMatrixInverse_2(disMatrix, nLen, disMatrix_r);
	for (size_t i = 0; i < nLen; i++)
	{
		for (size_t j = 0; j < nLen; j++)
		{
			cout << disMatrix_r[i][j] << "\t";
		}
		cout << endl;
	}
	cout << endl;
	multiply(disMatrix_r, f, w);
	for (size_t i = 0; i < nLen; i++)
	{
		cout << w[i][0] << endl;
	}
	// 2.generate result
	int nResultLen = 100;
	double dbStep = 4.0 / nResultLen;
	for (size_t i = 0; i < nResultLen; i++)
	{
		double x = -2 + dbStep*i;
		double y = 0;
		for (size_t j = 0; j < nLen; j++)
		{
			y += w[j][0] * funPhi(abs(x - _sequence[j].x));
		}
		_sequenceResult.push_back(DPoint3(x, y, 0));
	}
}


Sequence1DLayer::~Sequence1DLayer()
{
}
void Sequence1DLayer::Draw() {
	// draw sequence
	int nSeqLen = _sequence.size();
	if (nSeqLen > 0) {
		glBegin(GL_LINE_STRIP);
		for (size_t i = 0; i < nSeqLen; i++)
		{
			glVertex3d(_sequence[i].x, _sequence[i].y, _sequence[i].z);

		}
		glEnd();
		glColor3f(1, 0, 0);
		glBegin(GL_LINE_STRIP);
		nSeqLen = _sequenceResult.size();
		for (size_t i = 0; i < nSeqLen; i++)
		{
			glVertex3d(_sequenceResult[i].x, _sequenceResult[i].y, _sequenceResult[i].z);

		}
		glEnd();
	}
}