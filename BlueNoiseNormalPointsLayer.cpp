#include "BlueNoiseNormalPointsLayer.h"

#include <random>

using namespace std;

BlueNoiseNormalPointsLayer::BlueNoiseNormalPointsLayer(int number, double mx, double my, double vx, double vy)
	:SingleNormalPointsLayer(number, mx, my, vx, vy)
{
	generateNormalPoints(number, mx, my, vx, vy);
}


BlueNoiseNormalPointsLayer::~BlueNoiseNormalPointsLayer()
{
}

void BlueNoiseNormalPointsLayer::generateNormalPoints(int number, double mx, double my, double vx, double vy) {
	double dbSpatialScope =/*(number/10000.0)**/4.0;

	int nGeneratedPoints = 0;							// number of generated points
														// first point
	int nCount = 0;										// count the darting times
	int nMaxTime = 10000000;							// try this time at most
														
	double dbDis = dbSpatialScope / sqrt(number * 2);	// use a global distance

	//default_random_engine generator;//如果用这个默认的引擎，每次生成的随机序列是相同的。
	random_device rd;
	mt19937 gen(rd());
	//normal(0,1)中0为均值，1为方差
	normal_distribution<double> normalx(mx, vx);
	normal_distribution<double> normaly(my, vy);


	while (nGeneratedPoints<number&&nCount++<nMaxTime)
	{
		double x = normalx(gen);
		double y = normaly(gen);
		Point newPt(DPoint3(x, y, 0));
		if (distanceCheck(newPt,_points,dbDis))
		{
			_points.push_back(DPoint3(x, y, 0));
			nGeneratedPoints++;
		}

	}
}
