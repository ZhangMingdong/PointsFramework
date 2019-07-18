#include "GridLayer.h"


#include <QGLWidget>
#include <gl/GLU.h>
#include <QDebug>


GridLayer::GridLayer(int nPoints)
{
	// generate grid points
	// grid step
	double dbStep = .04;
	for (int i = 0; i < g_nLength; i++)
	{
		for (int j = 0; j < g_nLength; j++)
		{
			double x = (i - g_nLength / 2)*dbStep;
			double y = (j - g_nLength / 2)*dbStep;
			_gridRegular[i][j] = DPoint3(x, y, 0);
			_gridDistort[i][j] = DPoint3(x, y, 0);
		}

	}
}


GridLayer::~GridLayer()
{
}

void GridLayer::Draw() {
	// draw grid
	glBegin(GL_LINES);

	for (int i = 0; i < g_nLength; i++)
	{
		for (int j = 1; j < g_nLength; j++)
		{
			glVertex2d(_gridDistort[i][j - 1].x, _gridDistort[i][j - 1].y);
			glVertex2d(_gridDistort[i][j].x, _gridDistort[i][j].y);

			glVertex2d(_gridDistort[j - 1][i].x, _gridDistort[j - 1][i].y);
			glVertex2d(_gridDistort[j][i].x, _gridDistort[j][i].y);
		}
	}
	glEnd();
}




void GridLayer::Clear() {
}


void GridLayer::OnMouse(DPoint3 ptMouse) {
	// mouse move, fish eye effect
	double dbRadius = .2;
	for (int i = 0; i < g_nLength; i++)
	{
		for (int j = 0; j < g_nLength; j++)
		{
			DPoint3 pt = _gridRegular[i][j];
			double dbDis = (pt - ptMouse).norm();
			if (dbDis < dbRadius) {
				// original method, use sqrt
				if (false) {
					double dbScale = dbDis / dbRadius;
					double dbScaleNew = sqrt(dbScale);
					pt = ptMouse + (pt - ptMouse)*(dbScaleNew / dbScale);
				}

				// use formula from "Structure-aware Fisheye Views for Efficient Large Graph Exploration"
				{
					double dbM = 3;
					double dbScale = dbDis / dbRadius;
					double dbScaleNew = (dbM + 1)*dbScale / (dbM*dbScale + 1);
					pt = ptMouse + (pt - ptMouse)*(dbScaleNew/ dbScale);
				}
			}
			_gridDistort[i][j] = pt;
		}

	}
}