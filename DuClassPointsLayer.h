#pragma once
#include "ILayer.h"
#include <vector>
/*
	class used to represent points with dual class. This layer is use to test svm
*/
class DuClassPointsLayer :
	public ILayer
{
public:
	DuClassPointsLayer();
	virtual ~DuClassPointsLayer();
	virtual void Draw();
	virtual void AddPoint(Point pt, bool bRight = false);
	virtual void Clear();
	virtual void UpdateLayer();
private:

	// Point list
	std::vector<Point> _points;
	// another point list
	std::vector<Point> _pointsR;
	// the parameter vector
	double _arrW[3];
};

