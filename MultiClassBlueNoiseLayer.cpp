#include "MultiClassBlueNoiseLayer.h"
#include <QGLWidget>
#include <gl/GLU.h>

// weather using new approach
bool bNewApproach = true;

MultiClassBlueNoiseLayer::MultiClassBlueNoiseLayer(int nPoints) :RandomPointsLayer(0),_nClassShow(3)
{
	generatePoints(nPoints);
}

MultiClassBlueNoiseLayer::~MultiClassBlueNoiseLayer()
{
}

void MultiClassBlueNoiseLayer::Draw() {
	if (bNewApproach)
	{
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
	else {

		int nLen = _points.size();
		// draw points
		if (_nClassShow & 1)
		{
			glColor4f(0, 1, 1, 1);
			for (int i = 0; i<nLen; i++)
			{
				glBegin(GL_POINTS);
				glVertex3d(_points[i].x, _points[i].y, _points[i].z);
				glEnd();
			}
		}

		// draw the other class of points
		if (_nClassShow & 2) {
			int nLen2 = _points2.size();
			glColor4f(1, 0, 0, 1);
			for (int i = 0; i<nLen2; i++)
			{
				glBegin(GL_POINTS);
				glVertex3d(_points2[i].x, _points2[i].y, _points2[i].z);
				glEnd();
			}

		}
	}
}

void MultiClassBlueNoiseLayer::generatePoints(int nPoints) {
	_points.clear();
	_points2.clear();

	// spatial scope
	double dbSpatialScope = 4.0;
	if (bNewApproach)
	{

		int nClass = 3;

		double dbDisGlobal = dbSpatialScope / sqrt(nPoints * 2 * nClass);	// distance used when consider points belong to every class
		double dbDis = dbSpatialScope / sqrt(nPoints * 2);

		// add each class
		for (size_t i = 0; i < nClass; i++)
		{
			// vector for this class
			std::vector<Point> points;
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
	else
	{

		double dbDisGlobal = dbSpatialScope / sqrt(nPoints * 4);	// distance used when consider points belong to every class
		double dbDis = dbSpatialScope / sqrt(nPoints * 2);
		// 1.The first class

		int nGeneratedPoints = 1;				// number of generated points
												// first point
		_points.push_back(DPoint3(-dbSpatialScope / 2 + MyRandom()*dbSpatialScope, -dbSpatialScope / 2 + MyRandom()*dbSpatialScope, 0));
		int nCount = 0;							// count the darting times
		int nMaxTime = 10000000;				// try this time at most

												// use a global distance

		while (nGeneratedPoints<nPoints&&nCount++<nMaxTime)
		{
			DPoint3 ptNew(-dbSpatialScope / 2 + MyRandom()*dbSpatialScope, -dbSpatialScope / 2 + MyRandom()*dbSpatialScope, 0);
			int nLen = _points.size();
			bool bDiscard = false;	// whether discard this point
									// test the distance
			for (size_t i = 0; i < nLen; i++)
			{
				double x = _points[i].x - ptNew.x;
				double y = _points[i].y - ptNew.y;
				if (sqrt(x*x + y*y) < dbDis) {
					bDiscard = true;
					break;
				}
			}
			if (!bDiscard) {
				_points.push_back(ptNew);
				nGeneratedPoints++;
			}
		}
		// 2. the second class
		int nLen1 = _points.size();
		nGeneratedPoints = 0;					// number of generated points
		nCount = 0;								// count the darting times

		while (nGeneratedPoints<nPoints&&nCount++<nMaxTime)
		{
			DPoint3 ptNew(-dbSpatialScope / 2 + MyRandom()*dbSpatialScope, -dbSpatialScope / 2 + MyRandom()*dbSpatialScope, 0);
			bool bDiscard = false;	// whether discard this point
									// check for class 1
			for (size_t i = 0; i < nLen1; i++)
			{
				double x = _points[i].x - ptNew.x;
				double y = _points[i].y - ptNew.y;
				if (sqrt(x*x + y*y) < dbDisGlobal) {
					bDiscard = true;
					break;
				}
			}
			if (bDiscard) continue;
			// check for class 2
			int nLen2 = _points2.size();
			for (size_t i = 0; i < nLen2; i++)
			{
				double x = _points2[i].x - ptNew.x;
				double y = _points2[i].y - ptNew.y;
				if (sqrt(x*x + y*y) < dbDis) {
					bDiscard = true;
					break;
				}
			}
			if (bDiscard) continue;
			_points2.push_back(ptNew);
			nGeneratedPoints++;
		}
	}
}

/*
	example: 1,2,4,7
*/
void MultiClassBlueNoiseLayer::UpdateLayer() {
	if (bNewApproach) {

		int nLen = _vecPoints.size();
		if (_nClassShow == pow(2, nLen - 1))
		{
			// 4->7
			_nClassShow = _nClassShow * 2 - 1;
		}
		else if (_nClassShow > pow(2, nLen - 1))
		{
			// 7->1
			_nClassShow = 1;
		}
		else {
			_nClassShow *= 2;
		}
	}
	else {
		_nClassShow = (_nClassShow + 1) % (3) + 1;
	}
};