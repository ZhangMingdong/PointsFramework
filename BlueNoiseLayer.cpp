#include "BlueNoiseLayer.h"



BlueNoiseLayer::BlueNoiseLayer(int nPoints):RandomPointsLayer(0)
{
	generatePoints(nPoints);
}


BlueNoiseLayer::~BlueNoiseLayer()
{
}

void BlueNoiseLayer::generatePoints(int nPoints) {
	_points.clear();
	// spatial scope
	double dbSpatialScope =/*(number/10000.0)**/4.0;

	int nGeneratedPoints = 1;				// number of generated points
	// first point
	_points.push_back(DPoint3(-dbSpatialScope / 2 + MyRandom()*dbSpatialScope, -dbSpatialScope / 2 + MyRandom()*dbSpatialScope, 0));
	int nCount = 0;							// count the darting times
	int nMaxTime = 10000000;				// try this time at most

	// use a global distance
	double dbDis = dbSpatialScope / sqrt(nPoints * 2);

	while (nGeneratedPoints<nPoints&&nCount++<nMaxTime)
	{
		DPoint3 ptNew(-dbSpatialScope / 2 + MyRandom()*dbSpatialScope, -dbSpatialScope / 2 + MyRandom()*dbSpatialScope, 0);
		int nLen = _points.size();
		bool bDiscard = false;	// whether discard this point
		// original distance, using adaptive distance
		// double dbDis = dbSpatialScope / sqrt(nLen * 2);

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
}