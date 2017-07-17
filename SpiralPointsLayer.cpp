#include "SpiralPointsLayer.h"
#include <QGLWidget>
#include <gl/GLU.h>
#include <iostream>

#define TRAIN_ANN

using namespace std;

SpiralPointsLayer::SpiralPointsLayer():_arrLabels(0)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	_pInput = new MyMatrix(_nPoints, _nD);
	_pOutput = new MyMatrix(_nPoints, 1);

	generatePoints();

#ifdef TRAIN_ANN
	_pClassifier = IMyClassifier::CreateClassifier(IMyClassifier::Ann, _nPoints, _nD, _nClass,100);
#else
	_pClassifier = IMyClassifier::CreateClassifier(IMyClassifier::SoftMax, _nPoints, _nD, _nClass);
#endif
	showClassifier();


	// the classifier will be changed when pushing the update button
}

SpiralPointsLayer::~SpiralPointsLayer()
{
	delete _pInput;
	delete _pOutput;

	delete _pClassifier;

	if (_arrLabels) delete[] _arrLabels;
}

void SpiralPointsLayer::generatePoints() {
//	generateSpiralPoints();
	generateCircularPoints();


	for (size_t i = 0; i < _nPoints; i++)
	{
		for (size_t j = 0; j < _nD; j++)
		{
			_pInput->SetValue(i, j, _vecPoints[i]._arrCoord[j]);
		}
	}
	_arrLabels = new int[_nPoints];
	for (size_t i = 0; i < _nPoints; i++)
	{
		_arrLabels[i] = _vecPoints[i]._nLabel;

	}

}

void SpiralPointsLayer::generateSpiralPoints() {
	double dbBiasX = .1;
	double dbBiasY = .2;
	_vecPoints.clear();
	// generate spiral points
	for (size_t i = 0; i < _nClass; i++)
	{
		// for each class
		for (size_t j = 0; j < _nPointPerClass; j++)
		{
			// add each point
			double r = j / (double)(_nPointPerClass - 1);
			// in the web the delt is 0.2, but I use 0.002. No idea why
			double t = i * 4 + j*4.0 / (_nPointPerClass - 1) + rand() / (double)RAND_MAX*_nPointPerClass*0.002;

			_vecPoints.push_back(LabeledPoint(dbBiasX + r*sin(t), dbBiasY + r*cos(t), i));
		}
	}
}

void SpiralPointsLayer::generateCircularPoints() {
	double dbBiasX = .1;
	double dbBiasY = .2;
	_vecPoints.clear();
	// generate spiral points
	for (size_t i = 0; i < _nClass; i++)
	{
		// for each class
		for (size_t j = 0; j < _nPointPerClass; j++)
		{
			// add each point
			// the radius
			double r = (i + 1) / 5.0 + rand() / (double)RAND_MAX*0.02;
			// in the web the delt is 0.2, but I use 0.002. No idea why
			double t = 2 * 3.14159*j / _nPointPerClass;

			_vecPoints.push_back(LabeledPoint(dbBiasX + r*sin(t), dbBiasY + r*cos(t), i));
		}
		// add this line to bias the circles
		//dbBiasX += .5;
	}
}

void SpiralPointsLayer::Draw() {
	// draw the spiral points
	double colors[3][3] = {
		{1,0,0}
		,{0,1,0}
		,{0,0,1}
	};
	glPointSize(2.0f);
	glBegin(GL_POINTS);
	for each (LabeledPoint pt in _vecPoints)
	{
		glColor3dv(colors[pt._nLabel]);
		glVertex3d(pt._arrCoord[0], pt._arrCoord[1], 0);
	}
	glEnd();

	// draw background
	double dbOpacity = 0.5;
	double Rcolors[3][4] = {
		{ 1,0,0,dbOpacity }
		,{ 0,1,0,dbOpacity }
		,{ 0,0,1,dbOpacity }
	};
	glPointSize(2.0f);
	glBegin(GL_POINTS);
	for (size_t i = 0,length=_vecResultPt.size(); i < length; i++)
	{
		glColor4dv(Rcolors[_vecResultPt[i]._nLabel]);
		glVertex3d(_vecResultPt[i]._arrCoord[0], _vecResultPt[i]._arrCoord[1], 0);
	}
	glEnd();
}

void SpiralPointsLayer::Clear() {

}

void SpiralPointsLayer::train() {

	_pClassifier->Train(_pInput, _arrLabels);
}


void SpiralPointsLayer::showClassifier() {
	_vecResultPt.clear();
	// generate the result plot
	double xMin = _vecPoints[0]._arrCoord[0];
	double xMax = _vecPoints[0]._arrCoord[0];
	double yMin = _vecPoints[0]._arrCoord[1];
	double yMax = _vecPoints[0]._arrCoord[1];
	for each (LabeledPoint pt in _vecPoints)
	{
		if (pt._arrCoord[0] < xMin) xMin = pt._arrCoord[0];
		if (pt._arrCoord[0] > xMax) xMax = pt._arrCoord[0];
		if (pt._arrCoord[1] < yMin) yMin = pt._arrCoord[1];
		if (pt._arrCoord[1] > yMax) yMax = pt._arrCoord[1];
	}
	xMin = xMin - 1;
	xMax = xMax + 1;
	yMin = yMin - 1;
	yMax = yMax + 1;
	double h = 0.01;
	for (double x = xMin; x < xMax; x += h)
	{
		for (double y = yMin; y < yMax; y += h)
		{
			double X[_nD] = { x,y };
			_vecResultPt.push_back(LabeledPoint(x, y, _pClassifier->CalcLabel(X)));
		}
	}
}

void SpiralPointsLayer::UpdateLayer() {
	// train the parameter
	train();
	// reload the visualization of the classifier
	showClassifier();
}

