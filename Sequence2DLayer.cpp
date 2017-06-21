#include "Sequence2DLayer.h"

#include "ColorMap.h"
#include "tnt_array1d.h"
#include "tnt_array2d.h"

#include "jama_eig.h"

#include <QGLWidget>
#include <gl/GLU.h>
#include "MathFunction.h"

Sequence2DLayer::Sequence2DLayer():_dataTexture(0)
{
	_sequence.clear();
	_sequenceResult.clear();
	// 0.generation
	DPoint3 seq[10] = {
		DPoint3(-1.5, -1.5, 2)
		,DPoint3(-1.6, -0.6, 6)
		,DPoint3(-1.2, 0, 10)
		,DPoint3(-0.8, .3, 3)
		,DPoint3(-0.1, .5, 8)
		,DPoint3(-0.0, .7, 14)
		,DPoint3(0.4, .9, 6)
		,DPoint3(0.6, .5, 5)
		,DPoint3(1.2, .3, 2)
		,DPoint3(1.6, .0, 9)
	};
	int nLen = 10;
	for (size_t i = 0; i < nLen; i++)
	{
		_sequence.push_back(seq[i]);
	}
	int nResult = 100;
	// 1.calculation
	Array2D<double> disMatrix = Array2D<double>(nLen, nLen);	// distance matrix
	Array2D<double> disMatrix_r = Array2D<double>(nLen, nLen);	// reverse of distance matrix
	Array2D<double> f = Array2D<double>(nLen, 1);					// array of f
	Array2D<double> w = Array2D<double>(nLen, 1);					// array of w
	for (size_t i = 0; i < nLen; i++)
	{
		for (size_t j = 0; j < nLen; j++)
		{
			double dbX = _sequence[i].x - _sequence[j].x;
			double dbY = _sequence[i].y - _sequence[j].y;
			disMatrix[i][j] = funPhi(sqrt(dbX*dbX + dbY*dbY));
		}
	}
	for (size_t i = 0; i < nLen; i++)
	{
		f[i][0] = _sequence[i].z;
	}
	GetMatrixInverse_2(disMatrix, nLen, disMatrix_r);

	multiply(disMatrix_r, f, w);
	for (size_t i = 0; i < nLen; i++)
	{
		cout << w[i][0] << endl;
	}
	// 2.generate result
	int nResultLen = 100;
	double dbStep = 4.0 / nResultLen;
	for (size_t i = 0; i < nResultLen; i++)
	{
		for (size_t j = 0; j < nResultLen; j++)
		{
			double x = -2 + dbStep*j;
			double y = -2 + dbStep*i;
			double z = 0;
			for (size_t k = 0; k < nLen; k++)
			{
				double dbX = x - _sequence[k].x;
				double dbY = y - _sequence[k].y;
				z += w[k][0] * funPhi(sqrt(dbX*dbX + dbY*dbY));
			}
			_sequenceResult.push_back(DPoint3(x, y, z));

		}
	}

	// 3.generate texture
	_dataTexture = new GLubyte[4 * nResult*nResult];
	ColorMap* colormap = ColorMap::GetInstance();
	for (size_t i = 0; i < nResult; i++)
	{
		for (size_t j = 0; j < nResult; j++)
		{
			int nIndex = i*nResult + j;
			MYGLColor color = colormap->GetColor(_sequenceResult[nIndex].z);
			_dataTexture[4 * nIndex + 0] = color._rgb[0];
			_dataTexture[4 * nIndex + 1] = color._rgb[1];
			_dataTexture[4 * nIndex + 2] = color._rgb[2];
			_dataTexture[4 * nIndex + 3] = (GLubyte)255;
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nResult, nResult, 0, GL_RGBA, GL_UNSIGNED_BYTE, _dataTexture);


}


Sequence2DLayer::~Sequence2DLayer()
{
	if (_dataTexture)
	{
		delete[]_dataTexture;
	}
}

void Sequence2DLayer::Sequence2DLayer::Draw() {
	// draw 2d sequence
	int nSeqLen2D = _sequence.size();
	if (nSeqLen2D>0)
	{
		ColorMap* colormap = ColorMap::GetInstance();

		// draw points
		glPointSize(10);
		glBegin(GL_POINTS);
		for (size_t i = 0; i < nSeqLen2D; i++)
		{
			MYGLColor color = colormap->GetColor(_sequence[i].z);
			glColor3d(color._rgb[0] / 255.0, color._rgb[1] / 255.0, color._rgb[2] / 255.0);
			glVertex3d(_sequence[i].x, _sequence[i].y, 0);

		}
		glEnd();

		// draw field
		if (_bShowBackground) {

			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

			glBindTexture(GL_TEXTURE_2D, texID[0]);


			glBegin(GL_QUADS);
			float _fLeft = -2;
			float _fRight = 2;
			float _fBottom = -2;
			float _fTop = 2;
			glTexCoord2f(0.0f, 0.0f); glVertex2f(_fLeft, _fBottom);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(_fRight, _fBottom);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(_fRight, _fTop);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(_fLeft, _fTop);

			glEnd();

			glDisable(GL_TEXTURE_2D);
		}
	}
}