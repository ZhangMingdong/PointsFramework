#pragma once
#include "NormalPointsLayer.h"
/*
	Layer to show the points obey single normal distribution
	Mingdong
	2017/06/21
*/
class SingleNormalPointsLayer :
	public NormalPointsLayer
{
public:
	SingleNormalPointsLayer(int number, double mx, double my, double vx, double vy);
	virtual ~SingleNormalPointsLayer();
public:
	virtual void Draw();
};

