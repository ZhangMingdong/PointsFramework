#pragma once
#include "ILayer.h"
#include <vector>
/*
	Layer to show the spiral points and the softmax and ann classifier
*/
class SpiralPointsLayer : public ILayer
{
public:
	SpiralPointsLayer();
	virtual ~SpiralPointsLayer();
	virtual void Draw();
	virtual void Clear();
	virtual void UpdateLayer();
private:
	// number of points of each class
	int _nPointPerClass=100;
	// number of classes
	int _nClass = 3;
	std::vector<std::vector<Point> > _vecPoints;
};

