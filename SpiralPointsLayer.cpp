#include "SpiralPointsLayer.h"
#include <QGLWidget>
#include <gl/GLU.h>
#include <iostream>

#include "BPNeuralNetwork.h"
#include "TextureRenderer.h"

#define TRAIN_ANN


using namespace std;

// number of hidden layers
extern int g_nHiddenLayer;

SpiralPointsLayer::SpiralPointsLayer()
{
	//generatePoints();
}

SpiralPointsLayer::~SpiralPointsLayer()
{

}

void SpiralPointsLayer::generatePoints() {
	generateSpiralPoints();
//	generateCircularPoints();
//	generateMultiNormalPoints();
//	generateRectangularGridPoints();
//	generateCircularGridPoints();



}

void SpiralPointsLayer::generateSpiralPoints() {
	double dbBiasX = .1;
	double dbBiasY = .2;
	_vecPoints.clear();
	// generate spiral points
	for (size_t i = 0; i < _nClass; i++)
	{
		// for each class
		for (size_t j = 0; j < _nPointPerClass; j++)
		{
			// add each point
			double r = j / (double)(_nPointPerClass - 1);
			// in the web the delt is 0.2, but I use 0.002. No idea why
			double t = i * 4 + j*4.0 / (_nPointPerClass - 1) + rand() / (double)RAND_MAX*_nPointPerClass*0.002;

			_vecPoints.push_back(LabeledPoint(dbBiasX + r*sin(t), dbBiasY + r*cos(t), i));
		}
	}
}

void SpiralPointsLayer::generateMultiNormalPoints() {
	
	std::vector<DPoint3> pts[3];
	double arrMX[3] = { -1,1,0 };
	double arrMY[3] = { 1,1,-1 };
	double arrDX[3] = { .5,.5,.5 };
	double arrDY[3] = { .5,.5,.5 };
	for (size_t i = 0; i < 3; i++)
	{
		GenerateNormalPoints(pts[i], _nPointPerClass, arrMX[i], arrMY[i], arrDX[i], arrDY[i]);
		for each (DPoint3 pt in pts[i])
		{
			_vecPoints.push_back(LabeledPoint(pt.x,pt.y,i));
		}
	}
}

void SpiralPointsLayer::generateCircularPoints() {
	double dbBiasX = .1;
	double dbBiasY = .2;
	_vecPoints.clear();
	// generate spiral points
	for (size_t i = 0; i < _nClass; i++)
	{
		// for each class
		for (size_t j = 0; j < _nPointPerClass; j++)
		{
			// add each point
			// the radius
			double r = (i + 1) / 5.0 + rand() / (double)RAND_MAX*0.02;
			// in the web the delt is 0.2, but I use 0.002. No idea why
			double t = 2 * 3.14159*j / _nPointPerClass;

			_vecPoints.push_back(LabeledPoint(dbBiasX + r*sin(t), dbBiasY + r*cos(t), i));
		}
		// add this line to bias the circles
		dbBiasX += .5;
	}
}

void SpiralPointsLayer::generateRectangularGridPoints() {
	double xMin =  - 1;
	double xMax =  + 1;
	double yMin =  - 1;
	double yMax =  + 1;
	double h = 0.1;
	for (double x = xMin; x <= xMax; x += h)
	{
		for (double y = yMin; y <= yMax; y += h)
		{
			double X[_nD] = { x,y };
			int nLabel = 0;
			if (abs(x) < .5&&abs(y) < .5) {
				if (x > y) nLabel = 1;
				else nLabel = 2;
			}
			_vecPoints.push_back(LabeledPoint(x, y, nLabel));
		}
	}
}

void SpiralPointsLayer::generateCircularGridPoints() {
	double xMin = -1;
	double xMax = +1;
	double yMin = -1;
	double yMax = +1;
	double h = 0.1;
	for (double x = xMin; x <= xMax; x += h)
	{
		for (double y = yMin; y <= yMax; y += h)
		{
			double X[_nD] = { x,y };
			int nLabel = 0;
			if (x*x+y*y<0.64) {
				nLabel = 1;
				if (x*x + y*y < 0.16)
					nLabel = 2;
			}
			_vecPoints.push_back(LabeledPoint(x, y, nLabel));
		}
	}
}


