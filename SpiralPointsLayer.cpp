#include "SpiralPointsLayer.h"
#include <QGLWidget>
#include <gl/GLU.h>
#include <iostream>
using namespace std;

SpiralPointsLayer::SpiralPointsLayer()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_pW = new MyMatrix(_nD,_nClass);
	_pB = new MyMatrix(1,_nClass);

	generatePoints();
	initializeParams();
	showClassifier();
//	showClassifierAnn();
	// the classifier will be changed when pushing the update button
}

SpiralPointsLayer::~SpiralPointsLayer()
{
	delete _pW;
	delete _pB;
}

void SpiralPointsLayer::generatePoints() {
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

			_vecPoints.push_back(LabeledPoint(r*sin(t), r*cos(t),i));
		}
	}


}

void SpiralPointsLayer::initializeParams() {
	for (size_t j = 0; j < _nClass; j++)
	{
		_pB->SetValue(0, j, 1);
//		_arrB[j] = 1;
		for (size_t i = 0; i < _nD; i++)
		{
			_pW->SetValue(i, j, rand() / (double)RAND_MAX);
//			_arrW[i][j] = rand() / (double)RAND_MAX;
		}
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
	// train
	double dbStepSize = 1.0;
	double dbReg = 0.003;
	// set step to 1 to show the training procedure
	int nEpochs = 1;// 200;
	for (size_t i = 0; i < nEpochs; i++)
	{
		trainStep(dbStepSize, dbReg);
	}
}

void SpiralPointsLayer::trainStep(double dbStepSize, double dbReg) {	
	// size of traning data
	int nPoints = _vecPoints.size();						

	// 1.evaluate class scores
	double arrScores[_nPoints][_nClass];
	evaluateScore(arrScores);

	// 2.compute the class probabilities
	double arrExpScores[_nPoints][_nClass];
	double arrExpScoreSum[_nPoints];
	for (size_t i = 0; i < nPoints; i++)
	{
		arrExpScoreSum[i] = 0;
		for (size_t j = 0; j < _nClass; j++)
		{
			arrExpScores[i][j] = exp(arrScores[i][j]);
			arrExpScoreSum[i] += arrExpScores[i][j];
		}
	}
	double arrProb[_nPoints][_nClass];
	for (size_t i = 0; i < nPoints; i++)
	{
		for (size_t j = 0; j < _nClass; j++)
		{
			arrProb[i][j] = arrExpScores[i][j] / arrExpScoreSum[i];
		}
	}

	// 3.compute the loss: average cross-entropy loss and regularization
	double arrCorrectLogProb[_nPoints];
	double dbDataLoss = 0;
	for (size_t i = 0; i < nPoints; i++)
	{
		arrCorrectLogProb[i] = -log(arrProb[i][_vecPoints[i]._nLabel]);
		dbDataLoss += arrCorrectLogProb[i];
	}
	dbDataLoss /= nPoints;
	double dbRegLoss = 0;
	for (size_t j = 0; j < _nD; j++)
	{
		for (size_t k = 0; k < _nClass; k++)
		{
//			dbRegLoss += _arrW[j][k] * _arrW[j][k];
			dbRegLoss += _pW->GetValue(j, k)*_pW->GetValue(j, k);
		}
	}
	dbRegLoss *= (0.5*dbReg);
	double dbLoss = dbRegLoss + dbDataLoss;	
	cout << "Loss:\t"<<dbLoss << "\t";

	// 4.compute the  gradient on scores
	double arrDScore[_nPoints][_nClass];
	double dbCorrectProbs = 0;
	for (size_t j = 0; j < nPoints; j++)
	{
		for (size_t k = 0; k < _nClass; k++)
		{
			if (k== _vecPoints[j]._nLabel)
			{
				arrDScore[j][k] = arrProb[j][k]-1;
			}
			else {
				arrDScore[j][k] = arrProb[j][k];
			}
//			cout << arrDScore[j][k] << endl;
			arrDScore[j][k] /= nPoints;
		}
	}

	// 5.backpropagate the gradient to the parameters (W,b)
	double arrDW[_nD][_nClass];
	for (size_t i = 0; i < _nD; i++)
	{
		for (size_t j = 0; j < _nClass; j++)
		{
			arrDW[i][j] = 0;
			for (size_t k = 0; k < nPoints; k++)
			{
				arrDW[i][j] += _vecPoints[k]._arrCoord[i] * arrDScore[k][j];
			}
		}
	}
	double arrDB[_nClass];
	for (size_t j = 0; j < _nClass; j++)
	{
		arrDB[j] = 0;
		for (size_t k = 0; k < nPoints; k++)
		{
			arrDB[j] += arrDScore[j][k];
		}
	}

	// 6.regularization gradient
	for (size_t j = 0; j < _nD; j++) {
		for (size_t k = 0; k < _nClass; k++) {
			arrDW[j][k] += dbReg*_pW->GetValue(j, k);
		}
	}
	// 7.perform parameter update
	for (size_t j = 0; j < _nD; j++) {
		for (size_t k = 0; k < _nClass; k++) {
			_pW->SetValue(j, k, _pW->GetValue(j, k) - dbStepSize*arrDW[j][k]);
		}
	}
	for (size_t k = 0; k < _nClass; k++) {
		_pB->SetValue(0, k, _pB->GetValue(0, k) - dbStepSize*arrDB[k]);
	}

	// 8.calculate score and accuracy
	int nPredicted = 0;
	for each (LabeledPoint pt in _vecPoints)
	{
		if (calcLabel(pt._arrCoord) == pt._nLabel) nPredicted++;
	}
	cout<< "Accuracy:\t"<< nPredicted / (double)(_nPoints) << endl;;
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
			_vecResultPt.push_back(LabeledPoint(x, y, calcLabel(X)));
		}
	}
}

void SpiralPointsLayer::UpdateLayer() {
	// train the parameter
	train();
	// reload the visualization of the classifier
	showClassifier();
}

int SpiralPointsLayer::calcLabel(double* X) {
	double arrScore[_nClass];
	calcScore(X, arrScore);
	int nResult = 0;
	double dbMaxScore = arrScore[0];
	for (size_t i = 1; i < _nClass; i++)
	{
		if (arrScore[i] > dbMaxScore) {
			nResult = i;
			dbMaxScore = arrScore[i];
		}
	}
	return nResult;
}

void SpiralPointsLayer::calcScore(const double* X, double* arrScore) {
	for (size_t i = 0; i < _nClass; i++)
	{
		arrScore[i] = _pB->GetValue(0, i);
		for (size_t j = 0; j < _nD; j++)
		{
			arrScore[i] += X[j] * _pW->GetValue(j, i);
		}
	}
}

void SpiralPointsLayer::evaluateScore(double(*arrScores)[_nClass]) {
	for (size_t i = 0,length=_vecPoints.size(); i < length; i++)
	{
		calcScore(_vecPoints[i]._arrCoord, arrScores[i]);
	}
}


void SpiralPointsLayer::showClassifierAnn() {

}