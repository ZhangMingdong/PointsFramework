
#include "DRLayer.h"
#include <QGLWidget>
#include <gl/GLU.h>



#include <fstream>

#include "MyPCA.h"

#include <vector>
#include <iostream>

const bool c_bWater = false;			// using water data
const bool c_bNormalize = true;	// if normalize the data

using namespace std;

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
	if (c_bWater) {
		readWaterData();
		_nAttributes = 17;
	}
	else {
		readData();
		_nAttributes = 4;
	}

	if (c_bNormalize)regularize2();
	runDR();
}


DRLayer::~DRLayer()
{
}

void DRLayer::Draw() {
	// draw the spiral points
	double colors[][3] = {
		{ 1,0,0 }
		,{ 0,1,0 }
		,{ 0,0,1 }
		,{ 1,1,0 }
		,{ 0,1,1 }
	};
	glPointSize(4.0f);
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


void DRLayer::readData() {
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
	usingFirst2D();
//	usingPCA();
//	usingMDS();
}

void DRLayer::usingFirst2D() {
	for each (RawData dataitem in _vecRaw)
	{
		LabeledPoint pt(dataitem._arrBuf[0], dataitem._arrBuf[1]/2, dataitem._nLabel);
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