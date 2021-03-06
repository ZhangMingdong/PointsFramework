#include "ILayer.h"

#include "Sequence1DLayer.h"
#include "Sequence2DLayer.h"
#include "SingleNormalPointsLayer.h"
#include "MultiNormalPointsLayer.h"
#include "RandomPointsLayer.h"
#include "BlueNoiseLayer.h"
#include "BlueNoiseNormalPointsLayer.h"
#include "DuClassPointsLayer.h"
#include "SpiralPointsLayer.h"
#include "DRLayer.h"
#include "DataLayer.h"
#include "TimeSeriesLayer.h"
#include "GridLayer.h"

#include "LayerSetting.h"

#include <random>
using namespace std;


ILayer::ILayer(){
}


ILayer::~ILayer()
{
}

ILayer* ILayer::CreateLayer(EnumLayerType type, LayerSetting* pSetting,int nPoints,int nClass) {
	ILayer* pLayer = NULL;
	switch (type)
	{
	case ILayer::LT_Sequence_1D:
		pLayer= new Sequence1DLayer();
		break;
	case ILayer::LT_Sequence_2D:
		pLayer = new Sequence2DLayer();
		break;
	case ILayer::LT_Normal_Single:
		pLayer = new SingleNormalPointsLayer(nPoints);
		break;
	case ILayer::LT_Grid:
		pLayer = new GridLayer(1000);
		break;
	case ILayer::LT_Normal_Multi:
		pLayer = new MultiNormalPointsLayer(nPoints);
		break;
	case ILayer::LT_Normal_Blue:
		pLayer = new BlueNoiseNormalPointsLayer(nPoints);
		break;
	case ILayer::LT_Random:
		pLayer = new RandomPointsLayer(nPoints);
		break;
	case ILayer::LT_Random_Blue:
		pLayer = new BlueNoiseLayer(nPoints, nClass);
		break;
	case ILayer::LT_Random_Blue_Mult:
		break;
	case ILayer::LT_Dual:
		pLayer = new DuClassPointsLayer();
		break;
	case ILayer::LT_Spiral:
		pLayer = new SpiralPointsLayer();
		break;
	case ILayer::LT_DR:
		pLayer = new DRLayer();
		break;
	case ILayer::LT_Data:
		pLayer = new DataLayer();
		break;
	case ILayer::LT_Time_Series:
		pLayer = new TimeSeriesLayer();
		break;
	default:
		break;
	}
	if (pLayer) {

		pLayer->_pSetting = pSetting;
		pLayer->Initialize();
	}
	
	return pLayer;
}

double funPhi(double x,double r) {
//	return exp(-r*r );
//	return exp(-r*r * 4);
//	return exp(-r*r * 9);
//	return exp(-r*r * 16);
//	return exp(-r*r * 64);
	return exp(-x*x/(2*r*r));
//	return exp(-r*r / 4);
//	return exp(-r*r / 16);
}


void ILayer::DrawPoint(const DPoint3& p)
{
	glBegin(GL_POINTS);
	glVertex3d(p.x, p.y, p.z);
	glEnd();
}//----------------------------------------------------------------------------------------------------
void ILayer::DrawLine(const DPoint3& p1, const DPoint3& p2)
{
	glBegin(GL_LINE_STRIP);
	glVertex3d(p1.x, p1.y, 0);
	glVertex3d(p2.x, p2.y, 0);
	glEnd();
}//----------------------------------------------------------------------------------------------------

void ILayer::DrawLine(const std::vector<DPoint3> &dline, bool bClose)
{
	size_t size = dline.size();
	if (size < 2) return;
	bClose ? glBegin(GL_LINE_LOOP) : glBegin(GL_LINE_STRIP);
	for (size_t i = 0; i < size; i++)
	{
		glVertex3d(dline[i].x, dline[i].y, 0);
	}
	glEnd();
}//----------------------------------------------------------------------------------------------------

void ILayer::DrawCircle(DPoint3 center, double radius)
{
	std::vector<DPoint3> circle;
	double pi = 3.14;
	for (int i = 0; i < 360; i++)
	{
		DPoint3 pt;
		pt.x = center.x + radius*sin((double)i*3.14 / 180);
		pt.y = center.y + radius*cos((double)i*3.14 / 180);
		circle.push_back(pt);
	}
	DrawLine(circle, true);
}


bool ILayer::distanceCheck(const DPoint3& pt, const std::vector<DPoint3>& list, double dis) {
	int nLen = list.size();
	for (size_t i = 0; i < nLen; i++)
	{
		double x = list[i].x - pt.x;
		double y = list[i].y - pt.y;
		if (sqrt(x*x + y*y) < dis) {
			return false;
		}
	}
	return true;
}

void ILayer::GenerateNormalPoints(std::vector<DPoint3>& vecPts,int number, double mx, double my, double vx, double vy, double dbBiasX, double dbBiasY) {
	//default_random_engine generator;//如果用这个默认的引擎，每次生成的随机序列是相同的。
	random_device rd;
	mt19937 gen(rd());
	//normal(0,1)中0为均值，1为方差
	normal_distribution<double> normalx(mx, vx);
	normal_distribution<double> normaly(my, vy);

	for (int i = 0; i < number; i++)
	{
		double x = dbBiasX + normalx(gen);
		double y = dbBiasY + normaly(gen);
		vecPts.push_back(DPoint3(x, y, 0));
	}
}

// generate a normal sequence
vector<double> ILayer::generateNormalSequence(int nLen, double dbM, double dbV) {
	//default_random_engine generator;//如果用这个默认的引擎，每次生成的随机序列是相同的。
	random_device rd;
	mt19937 gen(rd());
	//normal(0,1)中0为均值，1为方差
	normal_distribution<double> normal(dbM,dbV);
	vector<double> vecResult;
	for (int i = 0; i < nLen; i++)
	{
		vecResult.push_back(normal(gen));
	}
	return vecResult;
}


std::vector<double> ILayer::generateRandomSequence(int nLen, double dbRange) {
	vector<double> vecResult;
	for (int i = 0; i < nLen; i++)
	{
		vecResult.push_back(rand() / (double)RAND_MAX*dbRange);
	}
	return vecResult;
}

// generate a list of points obey the random distribution
void ILayer::GenerateRandomPoints(std::vector<DPoint3>& vecPts, int number, double x, double y, double r) {
	for (int i = 0; i < number; i++)
	{
		double randomR = rand() / (double)RAND_MAX*r;
		double randomA = rand() / (double)RAND_MAX*PI2d;
		double randomX = x + randomR*sin(randomA);
		double randomY = y + randomR*cos(randomA);
		vecPts.push_back(DPoint3(randomX, randomY, 0));
	}
}

