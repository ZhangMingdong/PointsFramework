#include "NormalPointsLayer.h"


#include <random>

using namespace std;

NormalPointsLayer::NormalPointsLayer()
{
}


NormalPointsLayer::~NormalPointsLayer()
{
}

void NormalPointsLayer::Draw() {

}


void NormalPointsLayer::generateNormalPoints(int number, double mx, double my, double vx, double vy) {
	//default_random_engine generator;//如果用这个默认的引擎，每次生成的随机序列是相同的。
	random_device rd;
	mt19937 gen(rd());
	//normal(0,1)中0为均值，1为方差
	normal_distribution<double> normalx(mx, vx);
	normal_distribution<double> normaly(my, vy);

	for (int i = 0; i < number; i++)
	{
		double x = normalx(gen);
		double y = normaly(gen);
		_points.push_back(DPoint3(x, y, 0));
	}
}
