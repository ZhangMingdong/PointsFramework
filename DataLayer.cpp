#include "DataLayer.h"

#include <QGLWidget>
#include <gl/GLU.h>
#include <fstream>

#include "ConfidenceEllipse.h"

using namespace std;

int nDay = 31;
int nStep = 61;
int nEns = 50;
double colors[31][3];


int radius = 15;
int nLen = 2000;
double dbThreshold = .5;

// index of the focused day
int nFocusedDay = 5;

DataLayer::DataLayer()
{
	// generate color
	for (size_t i = 0; i < 31; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			colors[i][j] = rand() / (double)RAND_MAX;
		}
	}
	// points of the first class
	std::vector<LabeledPoint> vecPoint[6];

	ifstream input("../data/ensembleData31Day.txt");
//	int length = 94550;
	for (size_t iDay = 0; iDay < nDay; iDay++)
	{
//		int nLabel;
//		if (iDay < 10) nLabel = 0;
//		else if (iDay < 20) nLabel = 1;
//		else nLabel = 2;
		int nLabel=iDay;
		for (size_t iStep = 0; iStep < nStep; iStep++)
		{
//			int nLabel;
//			if (iStep < 20) nLabel = 0;
//			else if (iStep < 40) nLabel = 1;
//			else nLabel = 2;

			for (size_t iEns = 0; iEns < nEns; iEns++) {

				double x, y;
				input >> x >> y;
//				if (iEns == 0)	// first member only
//				if (iStep == 8)	// first step only

				if (iDay== nFocusedDay&&iStep==0) _vecPoints.push_back(LabeledPoint(x, y, 0));
				else if (iDay == nFocusedDay-1 && iStep == 4) _vecPoints.push_back(LabeledPoint(x, y, 1));
				else if (iDay == nFocusedDay-2 && iStep == 8) _vecPoints.push_back(LabeledPoint(x, y, 2));
				else if (iDay == nFocusedDay-3 && iStep == 12) _vecPoints.push_back(LabeledPoint(x, y, 3));
				else if (iDay == nFocusedDay-4 && iStep == 16) _vecPoints.push_back(LabeledPoint(x, y, 4));
				else if (iDay == nFocusedDay-5 && iStep == 20) _vecPoints.push_back(LabeledPoint(x, y, 5));
//				else if (iDay == nFocusedDay-6 && iStep == 24) _vecPoints.push_back(LabeledPoint(x, y, 6));
//				else if (iDay == nFocusedDay-7 && iStep == 28) _vecPoints.push_back(LabeledPoint(x, y, 7));
//				else if (iDay == nFocusedDay-8 && iStep == 32) _vecPoints.push_back(LabeledPoint(x, y, 8));


				if (iDay == nFocusedDay && iStep == 0) vecPoint[0].push_back(LabeledPoint(x, y, 0));
				else if (iDay == nFocusedDay - 1 && iStep == 4) vecPoint[1].push_back(LabeledPoint(x, y, 1));
				else if (iDay == nFocusedDay - 2 && iStep == 8) vecPoint[2].push_back(LabeledPoint(x, y, 2));
				else if (iDay == nFocusedDay - 3 && iStep == 12) vecPoint[3].push_back(LabeledPoint(x, y, 3));
				else if (iDay == nFocusedDay - 4 && iStep == 16) vecPoint[4].push_back(LabeledPoint(x, y, 4));
				else if (iDay == nFocusedDay - 5 && iStep == 20) vecPoint[5].push_back(LabeledPoint(x, y, 5));
			}
		}

	}
	// generate texture
	ConfidenceEllipse arrCE[6] = { ConfidenceEllipse(vecPoint[0])
		,ConfidenceEllipse(vecPoint[1])
		,ConfidenceEllipse(vecPoint[2])
		,ConfidenceEllipse(vecPoint[3])
		,ConfidenceEllipse(vecPoint[4])
		,ConfidenceEllipse(vecPoint[5])
	};
	// 3.generate texture
	_pTRenderer = new TextureRenderer(nLen, nLen);
	double dbStep = 2*radius / (double)nLen;
	for (size_t i = 0; i < nLen; i++)
	{
		double x = -radius + i*dbStep;
		for (size_t j = 0; j < nLen; j++)
		{
			double y = -radius + j*dbStep;
			GLubyte bufData[4] = { 0,0,0,(GLubyte)255 };
			GLubyte uColor = 255;
			for (size_t ii = 0; ii < 6; ii++)
			{
				double alpha = arrCE[ii].CalculateAlpha(DPoint3(x, y, 0));
				if(ii<3) bufData[ii] += alpha < dbThreshold ? (GLubyte)uColor : 0;
				else if (ii == 3) {
					bufData[0] += alpha < dbThreshold ? (GLubyte)uColor : 0;
					bufData[1] += alpha < dbThreshold ? (GLubyte)uColor : 0;
				}
				else if (ii == 4) {
					bufData[1] += alpha < dbThreshold ? (GLubyte)uColor : 0;
					bufData[2] += alpha < dbThreshold ? (GLubyte)uColor : 0;
				}
				else if (ii == 5) {
					bufData[2] += alpha < dbThreshold ? (GLubyte)uColor : 0;
					bufData[0] += alpha < dbThreshold ? (GLubyte)uColor : 0;
				}
			}


			int nIndex = j*nLen + i;
			_pTRenderer->SetTextureData(nIndex, bufData);
		}
	}
	_pTRenderer->GenerateTexture();

}


DataLayer::~DataLayer()
{
	delete _pTRenderer;
}
void DataLayer::Draw() {
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // GL_ONE_MINUS_SRC_ALPHA

	double colors[9][3] = {
		{ 1,0,0 }
		,{ 0,1,0 }
		,{ 0,0,1 }
		,{ 1,1,0 }
		,{ 0,1,1 }
		,{ 1,0,1 }
		,{ .7,.2,0 }
		,{ 0,.7,.2 }
		,{ .2,0,.7 }
	};

//	glPointSize(10.0f);
	glPointSize(5.0f);
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

	for (int i = -radius; i <= radius; i++)
	{
		glVertex2d(-radius, i);
		glVertex2d(radius, i);
		glVertex2d(i, -radius);
		glVertex2d(i, radius);
	}
	glEnd();

	// draw back ground
	float _fLeft = -radius;
	float _fRight = radius;
	float _fBottom = -radius;
	float _fTop = radius;
	_pTRenderer->Draw(_fLeft, _fRight, _fTop, _fBottom);

	return;
	// draw line
	glColor3f(1, 1, 1);
	glLineWidth(5.0);
	for (size_t iDay = 0; iDay < 5; iDay++)
	{
		glBegin(GL_LINE_STRIP);
		for (size_t iStep = 0; iStep < nStep; iStep++)
		{
			LabeledPoint pt = _vecPoints[iDay*nStep + iStep];
			glVertex3d(pt._arrCoord[0], pt._arrCoord[1], 0);
		}
		glEnd();
	}

}

