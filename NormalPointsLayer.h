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
	// DPoint3 list
	std::vector<DPoint3> _points;
	// grouped raw points
	std::vector<std::vector<DPoint3> > _groupedPoints;

	bool* _arrEdge=NULL;	// if there's a edge between two points
	void buildEdgeMatrix();	// build the edge matrix based on the points

};

