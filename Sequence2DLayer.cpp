#include "Sequence2DLayer.h"

#include "ColorMap.h"
#include "tnt_array1d.h"
#include "tnt_array2d.h"

#include "jama_eig.h"

#include <QGLWidget>
#include <gl/GLU.h>
#include "MathFunction.h"
#include "TextureRenderer.h"

Sequence2DLayer::Sequence2DLayer()
{

}

Sequence2DLayer::~Sequence2DLayer()
{


	if (_pTRenderer) delete[] _pTRenderer;
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
		if (_pSetting->_bShowBg) {
			float _fLeft = -_pSetting->_dbRadius;
			float _fRight = _pSetting->_dbRadius;
			float _fBottom = -_pSetting->_dbRadius;
			float _fTop = _pSetting->_dbRadius;
			_pTRenderer->Draw(_fLeft, _fRight, _fTop, _fBottom);
		}
	}
}

void Sequence2DLayer::Initialize() {
	_sequence.clear();
	// 0.generation

	/*	
	// random setted data set
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
	*/

	/*
	// unique value
	DPoint3 seq[10] = {
		DPoint3(-1.5, -1.5, 6)
		,DPoint3(-1.6, -0.6, 6)
		,DPoint3(-1.2, 0, 6)
		,DPoint3(-0.8, .3, 6)
		,DPoint3(-0.1, .5, 6)
		,DPoint3(-0.0, .7, 6)
		,DPoint3(0.4, .9, 6)
		,DPoint3(0.6, .5, 6)
		,DPoint3(1.2, .3, 6)
		,DPoint3(1.6, .0, 6)
	};
	*/

	//*
	double dbValue = 9.0;
	// unique value with new position
	DPoint3 seq[10] = {
		DPoint3(-1.5, 0, dbValue)
		,DPoint3(-0.2, 0, dbValue)
		,DPoint3(0.2, 0, dbValue)
		,DPoint3(1.5, 0, dbValue)
		,DPoint3(0, 1, dbValue)
		,DPoint3(0, -1, dbValue)
		,DPoint3(-1, .8, dbValue)
		,DPoint3(1, .8, dbValue)
		,DPoint3(-1, -.8, dbValue)
		,DPoint3(1, -.8, dbValue)
	};
	//*/
	int nLen = 10;
	for (size_t i = 0; i < nLen; i++)
	{
		_sequence.push_back(seq[i]);
	}
	runRDF();

	// 3.generate texture
	generateTexture();
}

void Sequence2DLayer::runRDF() {
	// 1.build the interpolator
	_interpolator.Build(_sequence, funPhi);
}

void Sequence2DLayer::generateTexture() {
	_pTRenderer = new TextureRenderer(_pSetting->_nResultLen, _pSetting->_nResultLen);
	ColorMap* colormap = ColorMap::GetInstance();
	// 2.generate result
	double dbStep = _pSetting->_dbRadius * 2 / _pSetting->_nResultLen;
	for (size_t i = 0; i < _pSetting->_nResultLen; i++)
	{
		for (size_t j = 0; j < _pSetting->_nResultLen; j++)
		{
			double x = -_pSetting->_dbRadius + dbStep*j;
			double y = -_pSetting->_dbRadius + dbStep*i;

			int nIndex = i*_pSetting->_nResultLen + j;
			MYGLColor color = colormap->GetColor(_interpolator.Calculate(x, y));
			GLubyte bufData[4] = { color._rgb[0]
				, color._rgb[1]
				, color._rgb[2]
				, (GLubyte)255 };
			_pTRenderer->SetTextureData(nIndex, bufData);

		}
	}
	_pTRenderer->GenerateTexture();

}

void Sequence2DLayer::SetMethod(int nMethod) {
	_nMethod = nMethod;
}