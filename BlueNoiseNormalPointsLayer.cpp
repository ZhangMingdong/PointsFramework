#include "BlueNoiseNormalPointsLayer.h"

#include <random>

using namespace std;

BlueNoiseNormalPointsLayer::BlueNoiseNormalPointsLayer(int number)
	:SingleNormalPointsLayer(number)
{
	generateNormalPoints(number, 0, 0, .4,.2);
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

	//default_random_engine generator;//��������Ĭ�ϵ����棬ÿ�����ɵ������������ͬ�ġ�
	random_device rd;
	mt19937 gen(rd());
	//normal(0,1)��0Ϊ��ֵ��1Ϊ����
	normal_distribution<double> normalx(mx, vx);
	normal_distribution<double> normaly(my, vy);


	while (nGeneratedPoints<number&&nCount++<nMaxTime)
	{
		double x = normalx(gen);
		double y = normaly(gen);
		DPoint3 newPt(DPoint3(x, y, 0));
		if (distanceCheck(newPt,_points,dbDis))
		{
			_points.push_back(DPoint3(x, y, 0));
			nGeneratedPoints++;
		}

	}
}
