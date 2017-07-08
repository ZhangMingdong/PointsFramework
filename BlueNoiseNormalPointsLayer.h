#pragma once
#include "SingleNormalPointsLayer.h"
/*
	Layer to show the points obey normal distribution with blue noise characteristic
	Mingdong
	2017/06/22
	*/
class BlueNoiseNormalPointsLayer :
	public SingleNormalPointsLayer
{
public:
	BlueNoiseNormalPointsLayer(int number, double mx, double my, double vx, double vy);
	virtual ~BlueNoiseNormalPointsLayer();
protected:
	// generate a list of points obey the normal distribution
	virtual void generateNormalPoints(int number, double mx, double my, double vx, double vy);
};

