#pragma once
#include "RandomPointsLayer.h"
/*
	Layer to show blue noise of multiple classes
	Mingdong
	2017/06/21
*/
class MultiClassBlueNoiseLayer :
	public RandomPointsLayer
{
public:
	MultiClassBlueNoiseLayer(int nPoints);
	virtual ~MultiClassBlueNoiseLayer();
public:
	virtual void Draw();
	virtual void UpdateLayer();
protected:
	// generate the points
	virtual void generatePoints(int nPoints);
private:
	// another Point list
	std::vector<Point> _points2;
	// which classes to show
	int _nClassShow;

	// array of the point list
	std::vector<std::vector<Point>> _vecPoints;
};

