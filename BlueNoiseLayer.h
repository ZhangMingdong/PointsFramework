#pragma once
#include "RandomPointsLayer.h"
/*
Layer to show blue noise of multiple classes
Mingdong
2017/06/21
*/
class BlueNoiseLayer : public RandomPointsLayer
{
public:
	BlueNoiseLayer(int nPoints, int nClass);
	virtual ~BlueNoiseLayer();
public:
	virtual void Draw();
	virtual void UpdateLayer();
protected:
	// generate the points
	virtual void generatePoints(int nPoints);
private:
	// which classes to show
	int _nClassShow = 0;

	// number of classes
	int _nClass = 0;

	// array of the point list
	std::vector<std::vector<DPoint3>> _vecPoints;
};

