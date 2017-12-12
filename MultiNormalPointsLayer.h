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
	MultiNormalPointsLayer(int number);
	virtual ~MultiNormalPointsLayer();
public:
	virtual void Draw();
	virtual void UpdateLayer();	
	virtual void AddPoint(DPoint3 pt, bool bRight = false);
	// clear the calculation and points for add points by hand
	virtual void Clear();
private:
	// number of clusters
	int _nCluster;
	// four cluster
	std::vector<std::vector<DPoint3> > _clusteredPoints;
};

