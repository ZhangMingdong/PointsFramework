#pragma once
#include "ILayer.h"
#include <vector>

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
	virtual void AddPoint(DPoint3 pt, bool bRight = false);
	virtual DistanceAndIndices Calculate(bool bDC);
	virtual void Clear();
protected:
	// DPoint3 list
	std::vector<DPoint3> _points;
	// another point list
	std::vector<DPoint3> _pointsR;

	// Ids of the nearest pair of points and their distance
	DistanceAndIndices _result;
	// if already calculated
	bool _bCalculated;
protected:
	// generate the points
	virtual void generatePoints(int nPoints);
	double MyRandom();
};

