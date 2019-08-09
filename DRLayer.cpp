
#include "DRLayer.h"
#include <QGLWidget>
#include <gl/GLU.h>



#include <fstream>

#include "MyPCA.h"

#include <vector>
#include <iostream>

#include "TextureRenderer.h"
#include "ColorMap.h"

#include <QDebug>

const bool c_bNormalize = true;	// if normalize the data

using namespace std;

const double c_dbK = 1.0 / sqrt(PI2d);
const double c_dbEpsilon = 1;
inline double KernelFun(double para) {
	return c_dbK * exp(-para * para / 2.0 / c_dbEpsilon / c_dbEpsilon);
}

int String2Label(string str) {
	if (str == "Iris-setosa") {
		return 0;
	}
	else if (str == "Iris-versicolor") {
		return 1;
	}
	else {
		return 2;
	}
}

DRLayer::DRLayer()
{

}

void DRLayer::generatePoints() {
	if (_pSetting->_enumData == LayerSetting::data_water) {
		readWaterData();
	}
	else {
		readIrisData();
	}

	if (c_bNormalize)
		regularize2();
	runDR();

	generateTextureByKDE();
}


//void DRLayer::UpdateLayer() {
//
//	generateTextureByKDE();
//}

DRLayer::~DRLayer()
{
	if (_pTRenderer) delete[] _pTRenderer;
}

/*
void DRLayer::Draw() {
	if (_pSetting->_bShowBg) {
		float _fLeft = -_pSetting->_dbRadius;
		float _fRight = _pSetting->_dbRadius;
		float _fBottom = -_pSetting->_dbRadius;
		float _fTop = _pSetting->_dbRadius;
		if (_pTRenderer)
			_pTRenderer->Draw(_fLeft, _fRight, _fTop, _fBottom);
	}

	// draw the spiral points
	double colors[][3] = {
		{ 1,0,0 }
		,{ 0,1,0 }
		,{ 0,0,1 }
		,{ 1,1,0 }
		,{ 0,1,1 }
	};
	glPointSize(_pSetting->_dbPointSize);
	glBegin(GL_POINTS);
	for each (LabeledPoint pt in _vecPoints)
	{
		glColor3dv(colors[pt._nLabel]);
		glVertex3d(pt._arrCoord[0], pt._arrCoord[1], 0);
	}
	glEnd();


	// coordinate
	glBegin(GL_LINES);
	glColor3d(.5, .5, .5);
	glVertex2d(1, 1);
	glVertex2d(1, -1);
	glVertex2d(0, 1);
	glVertex2d(0, -1);
	glVertex2d(-1, 1);
	glVertex2d(-1, -1);


	glVertex2d(1, 1);
	glVertex2d(-1, 1);
	glVertex2d(1, 0);
	glVertex2d(-1, 0);
	glVertex2d(1, -1);
	glVertex2d(-1, -1);
	glEnd();

}
*/
void DRLayer::readIrisData() {
	_nAttributes = 4;
	ifstream input("../Data/bezdekIris.data");
	while (!input.eof())
	{
		char c;
		RawData dataitem;
		for (size_t i = 0; i < 4; i++)
		{
			input >> dataitem._arrBuf[i]>>c;
		}
		string strLabel;
		input >> strLabel;
		dataitem._nLabel = String2Label(strLabel);
		_vecRaw.push_back(dataitem);
	}
}

void DRLayer::readWaterData() {
	_nAttributes = 17;

	ifstream input("../Data/water.csv");
	while (!input.eof())
	{
		char c;
		RawData dataitem;
		for (size_t i = 0; i < _nAttributes; i++)
		{
			input >> dataitem._arrBuf[i] >> c;
		}
		string strLabel;
		input >> strLabel;
		dataitem._nLabel = strLabel.c_str()[0]-'A';
		//if (dataitem._nLabel == 2) continue;
		_vecRaw.push_back(dataitem);
	}
}

void DRLayer::runDR() {
//	usingFirst2D();
//	usingPCA();
	usingMDS();
}

void DRLayer::usingFirst2D() {
	for each (RawData dataitem in _vecRaw)
	{
		LabeledPoint pt(dataitem._arrBuf[0], dataitem._arrBuf[1]/3, dataitem._nLabel);
		_vecPoints.push_back(pt);
	}
}

void DRLayer::usingPCA() {
	int mI = _nAttributes;
	int mO = 2;
	int n = _vecRaw.size();
	double* arrInput = new double[mI*n];
	double* arrOutput = new double[mO*n];
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < mI; j++)
		{
			arrInput[i*mI + j] = _vecRaw[i]._arrBuf[j];
		}
	}
	MyPCA pca;
	pca.DoPCA(arrInput, arrOutput, n, mI, mO, true);

	for (size_t i = 0; i < n; i++)
	{

		LabeledPoint pt(arrOutput[i*mO],arrOutput[i*mO+1], _vecRaw[i]._nLabel);
		_vecPoints.push_back(pt);
	}


	delete[] arrInput;
	delete[] arrOutput;
}

void DRLayer::regularize1() {
	double arrMean[4];	// range of the properties
	double arrMax[4];	// range of the properties
	double arrMin[4];	// range of the properties
	double arrMedian[4];
	for (size_t i = 0; i < 4; i++)
	{
		arrMax[i] = -1;
		arrMin[i] = INFINITY;
		arrMean[i] = 0;
	}
	for each (RawData dataitem in _vecRaw)
	{
		for (size_t i = 0; i < 4; i++)
		{
			arrMean[i] += dataitem._arrBuf[i];
			if (arrMax[i]<dataitem._arrBuf[i])
			{
				arrMax[i] = dataitem._arrBuf[i];
			}
			if (arrMin[i]>dataitem._arrBuf[i])
			{
				arrMin[i] = dataitem._arrBuf[i];
			}
		}
	}
	for (size_t i = 0; i < 4; i++)
	{
		arrMean[i] /= _vecRaw.size();
		arrMedian[i] = (arrMax[i] + arrMin[i]) / 2;
	}

	for (size_t i = 0,length=_vecRaw.size(); i < length; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			_vecRaw[i]._arrBuf[j] = (_vecRaw[i]._arrBuf[j] - arrMedian[j])*2 / arrMedian[j];
		}
	}
}

void DRLayer::regularize2() {
	double arrMean[17];	// range of the properties
	double arrVar[17];
	for (size_t i = 0; i < _nAttributes; i++)
	{
		arrMean[i] = 0;
		arrVar[i] = 0;
	}
	// calculate mean
	for each (RawData dataitem in _vecRaw)
	{
		for (size_t i = 0; i < _nAttributes; i++)
		{
			arrMean[i] += dataitem._arrBuf[i];
		}
	}
	for (size_t i = 0; i < _nAttributes; i++)
	{
		arrMean[i] /= _vecRaw.size();
	}
	// calculate variance
	for each (RawData dataitem in _vecRaw)
	{
		for (size_t i = 0; i < _nAttributes; i++)
		{
			double dbVar = dataitem._arrBuf[i] - arrMean[i];
			arrVar[i] += dbVar*dbVar;
		}
	}
	for (size_t i = 0; i < _nAttributes; i++)
	{
		arrVar[i] /= _vecRaw.size();
	}

	for (size_t i = 0, length = _vecRaw.size(); i < length; i++)
	{
		for (size_t j = 0; j < _nAttributes; j++)
		{
			_vecRaw[i]._arrBuf[j] = (_vecRaw[i]._arrBuf[j] - arrMean[j]) / sqrt(arrVar[j]);
		}
	}
}

void DRLayer::generateTextureByKDE() {
	double dbMaxR = 0;
	// 0.calculate radius
	for each (LabeledPoint pt in _vecPoints)
	{
		for (size_t i = 0; i < 1; i++)
		{
			double dbNewR = abs(pt._arrCoord[i]);
			if (dbNewR > dbMaxR) dbMaxR = dbNewR;
		}
	}
	_pSetting->_dbRadius = dbMaxR + 1;

	// 1.Create an instance of the texture renderer
	if (_pTRenderer) delete _pTRenderer;
	_pTRenderer = new TextureRenderer(_pSetting->_nResultLen, _pSetting->_nResultLen);


	// 2.generate texture data
	ColorMap* colormap = ColorMap::GetInstance();

	double dbStep = _pSetting->_dbRadius * 2 / _pSetting->_nResultLen;
	int nLen = _vecPoints.size();
	for (size_t i = 0; i < _pSetting->_nResultLen; i++)
	{
		for (size_t j = 0; j < _pSetting->_nResultLen; j++)
		{
			double x = -_pSetting->_dbRadius + dbStep * j;
			double y = -_pSetting->_dbRadius + dbStep * i;

			// for every grid points (x,y)

			int nIndex = i * _pSetting->_nResultLen + j;
			double dbDensity = 0.0;
			double B = _pSetting->_nH/20.0;

			// calculate the effection for each point
			for each (LabeledPoint pt in _vecPoints)
			{
				double dbX = x - pt._arrCoord[0];
				double dbY = y - pt._arrCoord[1];
				double dbR = sqrt(dbX*dbX + dbY * dbY);
				double kb = KernelFun(dbR / B) / B;
				dbDensity += kb;
			}
			dbDensity /= nLen;
			dbDensity *= 20;
			MYGLColor color = colormap->GetColor(dbDensity);
			GLubyte bufData[4] = { color._rgb[0]
				, color._rgb[1]
				, color._rgb[2]
				, (GLubyte)255 };
			_pTRenderer->SetTextureData(nIndex, bufData);

		}
	}
	_pTRenderer->GenerateTexture();
}