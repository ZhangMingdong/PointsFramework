#include "SingleNormalPointsLayer.h"

#include <QGLWidget>
#include <gl/GLU.h>
#include "tnt_array1d.h"
#include "tnt_array2d.h"

#include "jama_eig.h"

#include "MathFunction.h"

SingleNormalPointsLayer::SingleNormalPointsLayer(int number, double mx, double my, double vx, double vy):_pTRenderer(NULL)
, _pCEllipse(NULL)
{


	GenerateNormalPoints(_points,number, mx, my, vx, vy,.2,.4);

	generateResult();
}


SingleNormalPointsLayer::~SingleNormalPointsLayer()
{
	if (_pTRenderer) delete[] _pTRenderer;
	if (_pCEllipse) delete[] _pCEllipse;
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

	double radius = 4;
	float _fLeft = -radius;
	float _fRight = radius;
	float _fBottom = -radius;
	float _fTop = radius;
	_pTRenderer->Draw(_fLeft, _fRight, _fTop, _fBottom);

}


void SingleNormalPointsLayer::generateResult() {
	_pCEllipse = new ConfidenceEllipse(_points);


	// 4.generate result points
	_pointsResult.clear();

	double radius = 4;
	int nLen = 800;
	// 3.generate texture
	_pTRenderer=new TextureRenderer(nLen, nLen);
	for (size_t i = 0; i < nLen; i++)
	{
		double x = -radius + i*0.01;
		for (size_t j = 0; j < nLen; j++)
		{
			double y = -radius + j*0.01;

			double alpha = _pCEllipse->CalculateAlpha(DPoint3(x, y, 0));
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

			GLubyte bufData[4] = { (GLubyte)255, (GLubyte)255, 0, (GLubyte)(dbOpacity * 255) };
			_pTRenderer->SetTextureData(nIndex, bufData);
		}
	}
	_pTRenderer->GenerateTexture();
}