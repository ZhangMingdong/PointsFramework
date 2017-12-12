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
	virtual void AddPoint(DPoint3 pt, bool bRight = false);
	virtual void Clear();
	virtual void UpdateLayer();
private:

	// DPoint3 list
	std::vector<DPoint3> _points;
	// another point list
	std::vector<DPoint3> _pointsR;
	// the parameter vector
	double _arrW[3];
};

