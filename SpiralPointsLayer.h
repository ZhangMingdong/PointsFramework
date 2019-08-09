#pragma once
#include "ClassificationLayer.h"

/*
	Layer to generate the spiral points for classification
	Mingdong
	2019/08/09
*/
class SpiralPointsLayer : public ClassificationLayer
{
public:
	SpiralPointsLayer();
	virtual ~SpiralPointsLayer();
protected:
	// generate points and set them to the form of input data
	virtual void generatePoints();
private:

	// generate points of different circle
	void generateCircularPoints();

	// generate the spiral points
	void generateSpiralPoints();

	// generate points obey multi-normal distribution
	void generateMultiNormalPoints();

	// generate rectangular grid points
	void generateRectangularGridPoints();

	// generate circular grid points
	void generateCircularGridPoints();

};

