#pragma once
#include "RandomPointsLayer.h"
/*
	Layer to show the blue noise
	Mingdong
	2017/06/21
*/
class BlueNoiseLayer :
	public RandomPointsLayer
{
public:
	BlueNoiseLayer(int nPoints);
	virtual ~BlueNoiseLayer();
protected:
	// generate the points
	virtual void generatePoints(int nPoints);
};

