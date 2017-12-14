#include "BlueNoiseLayer.h"
#include <QGLWidget>
#include <gl/GLU.h>

// weather using new approach
bool bNewApproach = true;

BlueNoiseLayer::BlueNoiseLayer(int nPoints, int nClass) :RandomPointsLayer(0), _nClass(nClass), _nClassShow(nClass)
{
	generatePoints(nPoints);
}

BlueNoiseLayer::~BlueNoiseLayer()
{
}

void BlueNoiseLayer::Draw() {
	glPointSize(_pSetting->_dbPointSize);
	float colors[10][3] = {
		{ 1,0,0 }
		,{ 0,1,0 }
		,{ 0,0,1 }
		,{ 1,1,0 }
		,{ 0,1,1 }
		,{ 1,0,1 }
		,{ .5,.5,0 }
		,{ 0,.5,.5 }
		,{ .5,0,.5 }
		,{ 1,1,1 }
	};
	int nClass = _vecPoints.size();
	for (size_t i = 0; i < nClass; i++)
	{
		if (_nClassShow&(int)pow(2, i))
		{
			glColor3f(colors[i][0], colors[i][1], colors[i][2]);
			int nLen = _vecPoints[i].size();
			glBegin(GL_POINTS);
			for (int j = 0; j<nLen; j++)
			{
				glVertex3d(_vecPoints[i][j].x, _vecPoints[i][j].y, _vecPoints[i][j].z);
			}
			glEnd();

		}

	}
}

void BlueNoiseLayer::generatePoints(int nPoints) {

	// spatial scope
	double dbSpatialScope = 4.0;



	double dbDisGlobal = dbSpatialScope / sqrt(nPoints * 2 * _nClass);	// distance used when consider points belong to every class
	double dbDis = dbSpatialScope / sqrt(nPoints * 2);

	// add each class
	for (size_t i = 0; i < _nClass; i++)
	{
		// vector for this class
		std::vector<DPoint3> points;
		//		int nLen1 = _points.size();
		int nGeneratedPoints = 0;					// number of generated points
		int nCount = 0;								// count the darting times
		int nMaxTimes = 10000000;					// try this time at most

		while (nGeneratedPoints<nPoints&&nCount++<nMaxTimes)
		{
			DPoint3 ptNew(-dbSpatialScope / 2 + MyRandom()*dbSpatialScope, -dbSpatialScope / 2 + MyRandom()*dbSpatialScope, 0);
			bool bDiscard = false;	// whether discard this point
									// check for added classes
			for (size_t i = 0, length = _vecPoints.size(); i < length; i++)
			{
				for (size_t j = 0, length2 = _vecPoints[i].size(); j < length2; j++)
				{
					double x = _vecPoints[i][j].x - ptNew.x;
					double y = _vecPoints[i][j].y - ptNew.y;
					if (sqrt(x*x + y*y) < dbDisGlobal) {
						bDiscard = true;
						break;
					}
				}

			}
			if (bDiscard) continue;
			// check for class 2
			int nLen2 = points.size();
			for (size_t i = 0; i < nLen2; i++)
			{
				double x = points[i].x - ptNew.x;
				double y = points[i].y - ptNew.y;
				if (sqrt(x*x + y*y) < dbDis) {
					bDiscard = true;
					break;
				}
			}
			if (bDiscard) continue;
			points.push_back(ptNew);
			nGeneratedPoints++;
		}
		// add this class
		_vecPoints.push_back(points);

	}
}

/*
example: 1,2,4,7
*/
void BlueNoiseLayer::UpdateLayer() {
	if (_nClassShow == pow(2, _nClass - 1))
	{
		// 4->7
		_nClassShow = _nClassShow * 2 - 1;
	}
	else if (_nClassShow > pow(2, _nClass - 1))
	{
		// 7->1
		_nClassShow = 1;
	}
	else {
		_nClassShow *= 2;
	}
};