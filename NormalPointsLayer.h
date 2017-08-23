#pragma once
#include "ILayer.h"
#include <vector>
/*
	Layer to show the points obey normal distribution
	Mingdong
	2017/06/21
*/
class NormalPointsLayer :
	public ILayer
{
public:
	NormalPointsLayer();
	virtual ~NormalPointsLayer();
public:
	virtual void Draw();
protected:
	// Point list
	std::vector<Point> _points;
	// the points list of the covering result
	std::vector<Point> _pointsResult;
};

