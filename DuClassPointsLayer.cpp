#include "DuClassPointsLayer.h"
#include <QGLWidget>
#include <gl/GLU.h>


DuClassPointsLayer::DuClassPointsLayer()
{
	for (size_t i = 0; i < 3; i++)
	{
		_arrW[i] = rand() / (double)RAND_MAX;
	}
}


DuClassPointsLayer::~DuClassPointsLayer()
{
}

void DuClassPointsLayer::Draw() {
	glPointSize(2.0f);
	glBegin(GL_POINTS);
	int nLen = _points.size();
	// draw points
	glColor4f(0, 1, 1, 1);
	for (int i = 0; i<nLen; i++)
	{
		glVertex3d(_points[i].x, _points[i].y, _points[i].z);
	}


	// draw points of class two
	glColor4f(1, 0, 0, 1);
	int nLenR = _pointsR.size();
	for (int i = 0; i<nLenR; i++)
	{
		glVertex3d(_pointsR[i].x, _pointsR[i].y, _pointsR[i].z);
	}
	glEnd();

	// draw the line
	glColor4f(1, 1, 1, 1);
	double arrX[2] = { -5,5 };
	glBegin(GL_LINES);
	for (size_t i = 0; i < 2; i++)
	{
		glVertex2d(arrX[i], -(arrX[i] * _arrW[0] + _arrW[2]) / _arrW[1]);
	}
	glEnd();

}

/*
	update the parateters
	nClass=1/-1;
*/
void updateW(double* pW, Point pt, int nClass,double dbDelta,double dbLambda) {
	double dbBias = pW[0] * pt.x + pW[1] * pt.y + pW[2] - nClass*dbDelta;
	if (nClass*dbBias<0)
	{
		pW[0] -= pt.x*dbBias*dbLambda;
		pW[1] -= pt.y*dbBias*dbLambda;
		pW[2] -= dbBias*dbLambda;
	}
}

void DuClassPointsLayer::UpdateLayer() {
	// update parameter
	int nEpochs = 100;
	double dbDelta = 0.1;
	double dbLambda = 0.1;
	for (size_t i = 0; i < nEpochs; i++)
	{
		for (size_t j = 0,length=_points.size(); j < length; j++)
		{
			Point pt = _points[j];
			updateW(_arrW, pt, 1, dbDelta, dbLambda);
		}
		for (size_t j = 0, length = _pointsR.size(); j < length; j++)
		{
			Point pt = _pointsR[j];
			updateW(_arrW, pt, -1, dbDelta, dbLambda);
		}
	}
}

void DuClassPointsLayer::Clear() {
	_points.clear();
	_pointsR.clear();
}

void DuClassPointsLayer::AddPoint(Point pt, bool bRight) {
	if (bRight)
	{
		_pointsR.push_back(pt);
	}
	else {
		_points.push_back(pt);
	}
};