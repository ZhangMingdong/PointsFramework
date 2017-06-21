#include "SingleNormalPointsLayer.h"

#include <QGLWidget>
#include <gl/GLU.h>
#include "tnt_array1d.h"
#include "tnt_array2d.h"

#include "jama_eig.h"

#include "MathFunction.h"

SingleNormalPointsLayer::SingleNormalPointsLayer(int number, double mx, double my, double vx, double vy)
{

	_points.clear();

	generateNormalPoints(number, mx, my, vx, vy);

	// calculate the confidence eclipse
	const int row = number;
	const int col = 2;

	Array2D<double> d(row, col);
	for (int i = 0; i < row; i++)
	{
		d[i][0] = _points[i].x;
		d[i][1] = _points[i].y;
	}
	// 2.compute covariance matrix
	Array2D<double> covar_matrix(col, col);
	compute_covariance_matrix(d, covar_matrix);
	Array2D<double> covar_matrix_r(col, col);

	// 3.compute the reverse matrix of covariance
	GetMatrixInverse(covar_matrix, col, covar_matrix_r);

	// 4.generate result points
	_pointsResult.clear();
	Array2D<double> m0(1, 2);
	Array2D<double> m1(2, 1);
	Array2D<double> m(1, 2);
	Array2D<double> mr(1, 1);
	double radius = 4;
	for (double i = -radius; i < radius; i += 0.01)
	{
		for (double j = -radius; j < radius; j += 0.01)
		{
			m0[0][0] = m1[0][0] = i;
			m0[0][1] = m1[1][0] = j;
			multiply(m0, covar_matrix_r, m);
			multiply(m, m1, mr);
			double alpha = sqrt(mr[0][0]);
			if (alpha<1.5)
			{
				_pointsResult.push_back(DPoint3(i, j, 0));
			}
		}
	}
}


SingleNormalPointsLayer::~SingleNormalPointsLayer()
{
}

void SingleNormalPointsLayer::Draw() {

	int nLen = _points.size();
	// draw points
	for (int i = 0; i<nLen; i++)
	{
		glBegin(GL_POINTS);
		glVertex3d(_points[i].x, _points[i].y, _points[i].z);
		glEnd();
	}
	// draw the eclipse
	if (_bShowBackground) {
		glColor4f(1, 0, 0, 1);
		int nLenResult = _pointsResult.size();
		for (int i = 0; i<nLenResult; i++)
		{
			glBegin(GL_POINTS);
			glVertex3d(_pointsResult[i].x, _pointsResult[i].y, _pointsResult[i].z);
			glEnd();
		}
	}

}