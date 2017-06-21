#pragma once
#include "NormalPointsLayer.h"
/*
	Layer to show the points obey multi-normal distribution
	Mingdong
	2017/06/21
*/
class MultiNormalPointsLayer :
	public NormalPointsLayer
{
public:
	MultiNormalPointsLayer(int number, double mx, double my, double vx, double vy);
	virtual ~MultiNormalPointsLayer();
public:
	virtual void Draw();
private:
	// number of clusters
	int _nCluster;
	// four cluster
	std::vector<std::vector<Point> > _clusteredPoints;
};

