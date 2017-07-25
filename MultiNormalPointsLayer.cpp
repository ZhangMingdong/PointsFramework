#include "MultiNormalPointsLayer.h"

#include <QGLWidget>
#include <gl/GLU.h>


#include "AHCClustering.h"
#include "KMeansClustering.h"

MultiNormalPointsLayer::MultiNormalPointsLayer(int number, double mx, double my, double vx, double vy):_nCluster(0)
{
	_points.clear();
	_nCluster = 4;

	bool bMVN = true;
	if (bMVN) {
		double dbScale = 3;

		GenerateNormalPoints(_points, number / _nCluster, mx - vx * dbScale, my - vy * dbScale, vx, vy);
		GenerateNormalPoints(_points,number / _nCluster, mx - vx * dbScale, my + vy * dbScale, vx, vy);
		GenerateNormalPoints(_points,number / _nCluster, mx + vx * dbScale, my + vy * dbScale, vx, vy);
		GenerateNormalPoints(_points,number / _nCluster, mx + vx * dbScale, my - vy * dbScale, vx, vy);
	}
	else {
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

	//	AHCClustering clustering;
	KMeansClustering clustering;
	double* arrBuf = new double[number * 2];
	for (size_t i = 0; i < number; i++)
	{
		arrBuf[i * 2] = _points[i].x;
		arrBuf[i * 2 + 1] = _points[i].y;
	}
	int* arrLabel = new int[number];
	clustering.DoCluster(number, 2, _nCluster, arrBuf, arrLabel);
	_clusteredPoints.clear();
	for (size_t i = 0; i < _nCluster; i++)
	{
		_clusteredPoints.push_back(std::vector<Point>());
	}
	for (size_t i = 0; i < number; i++)
	{
		_clusteredPoints[arrLabel[i]].push_back(_points[i]);
	}

	delete arrBuf;
	delete arrLabel;
}


MultiNormalPointsLayer::~MultiNormalPointsLayer()
{
}

void MultiNormalPointsLayer::Draw() {
	RGBAf arrColors[4] = {
		RGBAf(1, 0, 0, 1)
		,RGBAf(0, 1, 0, 1)
		,RGBAf(0, 0, 1, 1)
		,RGBAf(1, 0, 1, 1)
	};
	for (size_t i = 0; i < _nCluster; i++)
	{
		glColor4f(arrColors[i].r, arrColors[i].g, arrColors[i].b, arrColors[i].a);
		glBegin(GL_POINTS);
		for (size_t j = 0; j<_clusteredPoints[i].size(); j++)
		{
			glVertex3d(_clusteredPoints[i][j].x, _clusteredPoints[i][j].y, _clusteredPoints[i][j].z);
		}
		glEnd();
	}
}