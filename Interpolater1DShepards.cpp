#include "Interpolater1DShepards.h"



Interpolater1DShepards::Interpolater1DShepards(std::vector<DPoint3> vecSamples, double dbP) :Interpolater1D(vecSamples, dbP)
{
}


Interpolater1DShepards::~Interpolater1DShepards()
{
}

double Interpolater1DShepards::Interpolate(double x) {
	double y = 0;
	double dbDisSum = 0;
	double dbMax = 10000000;
	double dbMin = .0000001;
	for (DPoint3 pt : _vecSamples) {
		double dbX = abs(x - pt.x);
		double dbDis = (dbX>dbMin) ? pow(dbX, -_dbP) : dbMax;
		dbDisSum += dbDis;
		y += dbDis*pt.y;
	}
	y /= dbDisSum;
	return y;
}