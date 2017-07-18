#include "SingleNormalPointsLayer.h"

#include <QGLWidget>
#include <gl/GLU.h>
#include "tnt_array1d.h"
#include "tnt_array2d.h"

#include "jama_eig.h"

#include "MathFunction.h"

SingleNormalPointsLayer::SingleNormalPointsLayer(int number, double mx, double my, double vx, double vy):_dataTexture(NULL)
{


	generateNormalPoints(number, mx, my, vx, vy,.2,.4);

	generateResult();
}


SingleNormalPointsLayer::~SingleNormalPointsLayer()
{
	if (_dataTexture) delete[] _dataTexture;
}

void SingleNormalPointsLayer::Draw() {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // GL_ONE_MINUS_SRC_ALPHA
	glBegin(GL_POINTS);
	int nLen = _points.size();
	// draw points
	glColor4f(0, 1, 1, 1);
	for (int i = 0; i<nLen; i++)
	{
		glVertex3d(_points[i].x, _points[i].y, _points[i].z);
	}
	// draw the eclipse
	if (_bShowBackground) {
		int nLenResult = _pointsResult.size();
		for (int i = 0; i<nLenResult; i++)
		{
			glColor4f(1, 0, 0, _pointsResult[i].z);
			glVertex3d(_pointsResult[i].x, _pointsResult[i].y,0);
		}
	}
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, texID[0]);

	double radius = 4;
	glBegin(GL_QUADS);
	float _fLeft = -radius;
	float _fRight = radius;
	float _fBottom = -radius;
	float _fTop = radius;
	glTexCoord2f(0.0f, 0.0f); glVertex2f(_fLeft, _fBottom);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(_fRight, _fBottom);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(_fRight, _fTop);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(_fLeft, _fTop);

	glEnd();

	glDisable(GL_TEXTURE_2D);
}


void SingleNormalPointsLayer::generateResult() {
	// calculate the confidence eclipse
	const int row = _points.size();
	const int col = 2;

	double dbMeanX = 0;
	double dbMeanY = 0;

	for (int i = 0; i < row; i++)
	{
		dbMeanX += _points[i].x;
		dbMeanY += _points[i].y;
	}
	dbMeanX /= row;
	dbMeanY /= row;



	Array2D<double> d(row, col);
	for (int i = 0; i < row; i++)
	{
		d[i][0] = _points[i].x - dbMeanX;
		d[i][1] = _points[i].y - dbMeanY;
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
	int nLen = 800;
	// 3.generate texture
	_dataTexture = new GLubyte[4 * nLen*nLen];
	for (size_t i = 0; i < nLen; i++)
	{
		double x = -radius + i*0.01;
		for (size_t j = 0; j < nLen; j++)
		{
			double y = -radius + j*0.01;
			m0[0][0] = m1[0][0] = x - dbMeanX;
			m0[0][1] = m1[1][0] = y - dbMeanY;
			multiply(m0, covar_matrix_r, m);
			multiply(m, m1, mr);
			double alpha = sqrt(mr[0][0]);
			double dbOpacity = 1 - alpha / 5.0;
			if (dbOpacity < 0) dbOpacity = 0;

			// original codes, just choose an ecclipse
			if (alpha<1.5)
			{
				_pointsResult.push_back(DPoint3(x, y, 1));
			}
			// new codes, use opacity
			//			if (alpha<5)
			//			{
			//				_pointsResult.push_back(DPoint3(i, j, dbOpacity));
			//			}

			int nIndex = j*nLen + i;
			_dataTexture[4 * nIndex + 0] = (GLubyte)255;
			_dataTexture[4 * nIndex + 1] = (GLubyte)255;
			_dataTexture[4 * nIndex + 2] = 0;
			_dataTexture[4 * nIndex + 3] = (GLubyte)(dbOpacity*255);
		}
	}

	glGenTextures(1, &texID[0]);
	glBindTexture(GL_TEXTURE_2D, texID[0]);
	// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nLen, nLen, 0, GL_RGBA, GL_UNSIGNED_BYTE, _dataTexture);
}