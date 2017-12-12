#include "MultiNormalPointsLayer.h"

#include <QGLWidget>
#include <gl/GLU.h>
#include <QDebug>

#include "AHCClustering.h"
#include "KMeansClustering.h"
#include "DBSCANClustering.h"

#include "ColorMap.h"


MultiNormalPointsLayer::MultiNormalPointsLayer(int number):_nCluster(0)
{
	_nCluster = 4;
	double mx=0;
	double my=0;
	double vx=.4;
	double vy=.2;

	// 1.generate points
	bool bMVN = true;
	if (bMVN) {
		double dbScale = 2;

		GenerateNormalPoints(_points, number / _nCluster, mx - vx * dbScale, my - vy * dbScale, vx, vy);
		GenerateNormalPoints(_points,number / _nCluster, mx - vx * dbScale, my + vy * dbScale, vx, vy);
		GenerateNormalPoints(_points,number / _nCluster, mx + vx * dbScale, my + vy * dbScale, vx, vy);
		GenerateNormalPoints(_points,number / _nCluster, mx + vx * dbScale, my - vy * dbScale, vx, vy);
	}
	else {
		// grid points
		number = 200;
		for (size_t i = 0; i < 10; i++)
		{

			for (size_t j = 0; j < 10; j++)
			{
				_points.push_back(DPoint3(i*0.1, j*0.1, 0.0));
				_points.push_back(DPoint3(i*0.1 - 1.55, j*0.1, 0.0));
			}
		}
	}
}


MultiNormalPointsLayer::~MultiNormalPointsLayer()
{
}

void MultiNormalPointsLayer::Draw() {
	glPointSize(_pSetting->_dbPointSize);
	// 2.draw clustered points
	if (_clusteredPoints.size()>0)
	{
		RGBAf arrColors[4] = {
			RGBAf(1, 0, 0, 1)
			,RGBAf(0, 1, 0, 1)
			,RGBAf(0, 0, 1, 1)
			,RGBAf(1, 0, 1, 1)
		};
		for (size_t i = 0; i < _nCluster; i++)
		{
			int x = i * 100;
			int r = x < 256 ? 255 - x : x < 512 ? 0 : x - 512;
			int g = x < 256 ? x : x < 512 ? 512 - x : 0;
			int b = x < 256 ? 0 : x < 512 ? x - 256 : 768 - x;
			glColor3f(r / 255.0, g / 255.0, b / 255.0);

			//glColor4f(arrColors[i].r, arrColors[i].g, arrColors[i].b, arrColors[i].a);
			glBegin(GL_POINTS);
			for (size_t j = 0; j<_clusteredPoints[i].size(); j++)
			{
				glVertex3d(_clusteredPoints[i][j].x, _clusteredPoints[i][j].y, _clusteredPoints[i][j].z);
			}
			glEnd();
		}
	}
	// 1.draw raw points
	if (_pSetting->_bShowBg) {
		glColor3f(.9, .9, .9);
		glBegin(GL_POINTS);
		for (size_t i = 0, length = _points.size(); i < length; i++)
		{
			glVertex3d(_points[i].x, _points[i].y, _points[i].z);
		}
		glEnd();

		/*
		for (size_t i = 0, length = _points.size(); i < length; i++)
		{
			DrawCircle(_points[i], .1);
		}
		*/
	}
}

void MultiNormalPointsLayer::UpdateLayer() {
	int number = _points.size();

	CLUSTER::Clustering* pClusterer = NULL;
	switch (_pSetting->_nClusteringMethod)
	{
	case 0:
		_nCluster = 4;
		pClusterer = new CLUSTER::AHCClustering();
		break;
	case 1:
		_nCluster = 4;
		pClusterer = new CLUSTER::KMeansClustering();
		break;
	case 2:
		{
			CLUSTER::DBSCANClustering* pSBSCAN = new CLUSTER::DBSCANClustering();
			pSBSCAN->SetDBSCANParams(_pSetting->_nMinPts, _pSetting->_dbEps);
			pClusterer = pSBSCAN;
		}
		break;
	default:
		return;
		break;
	}
	double* arrBuf = new double[number * 2];
	for (size_t i = 0; i < number; i++)
	{
		arrBuf[i * 2] = _points[i].x;
		arrBuf[i * 2 + 1] = _points[i].y;
	}
	int* arrLabel = new int[number];
	_nCluster = pClusterer->DoCluster(number, 2, _nCluster, arrBuf, arrLabel);

	// 2.generate clusters
	_clusteredPoints.clear();
	for (size_t i = 0; i < _nCluster; i++)
	{
		_clusteredPoints.push_back(std::vector<DPoint3>());
	}
	for (size_t i = 0; i < number; i++)
	{
		//		qDebug() << arrLabel[i];
		if (arrLabel[i] >= 0)
		{
			_clusteredPoints[arrLabel[i]].push_back(_points[i]);

		}
		else
			qDebug() << arrLabel[i];
	}
	qDebug() << _clusteredPoints.size();
	delete pClusterer;
	delete arrBuf;
	delete arrLabel;
}

void MultiNormalPointsLayer::AddPoint(DPoint3 pt, bool bRight) {
	_points.push_back(pt);
}

// clear the calculation and points for add points by hand
void MultiNormalPointsLayer::Clear() {
	_points.clear();
	_clusteredPoints.clear();
}