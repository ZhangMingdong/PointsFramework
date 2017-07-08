#pragma once
#include "ILayer.h"
#include <vector>
#include "MathTypes.h"

/*
	Layer to show the random points
	Mingdong
	2017/06/21
*/
class RandomPointsLayer :
	public ILayer
{
public:
	RandomPointsLayer(int nPoints);
	virtual ~RandomPointsLayer();
public:
	virtual void Draw();
	virtual void AddPoint(Point pt, bool bRight = false);
	virtual DistanceAndIndices Calculate(bool bDC);
	virtual void Clear();
protected:
	// Point list
	std::vector<Point> _points;
	// another point list
	std::vector<Point> _pointsR;
	// the points list of the covering result
	std::vector<Point> _pointsResult;
	// Ids of the nearest pair of points and their distance
	DistanceAndIndices _result;
	// if already calculated
	bool _bCalculated;
protected:
	// generate the points
	virtual void generatePoints(int nPoints);
	double MyRandom();
};

