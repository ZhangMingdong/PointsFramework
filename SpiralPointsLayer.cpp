#include "SpiralPointsLayer.h"
#include <QGLWidget>
#include <gl/GLU.h>
#include <iostream>

//#define TRAIN_ANN

using namespace std;

SpiralPointsLayer::SpiralPointsLayer()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_pW1 = new MyMatrix(_nD, _nHidden);
	_pB1 = new MyMatrix(1, _nHidden);
	_pW2 = new MyMatrix(_nHidden, _nClass);
	_pB2 = new MyMatrix(1, _nClass);
	_pHidden = new MyMatrix(_nPoints, _nHidden);
	_pInput = new MyMatrix(_nPoints, _nD);
	_pOutput = new MyMatrix(_nPoints, 1);

	_pSoftMax = new SoftMaxClassifier(_nPoints, _nD, _nClass);

	generatePoints();

#ifdef TRAIN_ANN
	initializeParamsAnn();
	showClassifierAnn();
#else
	initializeParams();
	showClassifier();
#endif


	// the classifier will be changed when pushing the update button
}

SpiralPointsLayer::~SpiralPointsLayer()
{

	delete _pW1;
	delete _pB1;
	delete _pW2;
	delete _pB2;

	delete _pHidden;

	delete _pInput;
	delete _pOutput;

	delete _pSoftMax;
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


	for (size_t i = 0; i < _nPoints; i++)
	{
		for (size_t j = 0; j < _nD; j++)
		{
			_pInput->SetValue(i, j, _vecPoints[i]._arrCoord[j]);
		}
	}
	for (size_t i = 0; i < _nPoints; i++)
	{
	}

}

void SpiralPointsLayer::initializeParams() {
}

void SpiralPointsLayer::initializeParamsAnn() {
	_pB1->InitZero();
	_pW1->InitRandom();
	_pB2->InitZero();
	_pW2->InitRandom();
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
	int* arrLabel = new int[_nPoints];
	for (size_t i = 0; i < _nPoints; i++)
	{
		arrLabel[i] = _vecPoints[i]._nLabel;

	}
	_pSoftMax->Train(_pInput, arrLabel);
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
			_vecResultPt.push_back(LabeledPoint(x, y, _pSoftMax->CalcLabel(X)));
		}
	}
}

void SpiralPointsLayer::UpdateLayer() {

#ifdef TRAIN_ANN
	// train the parameter
	trainAnn();
	// reload the visualization of the classifier
	showClassifierAnn();
#else
	// train the parameter
	train();
	// reload the visualization of the classifier
	showClassifier();
#endif
}


void SpiralPointsLayer::showClassifierAnn() {
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
			_vecResultPt.push_back(LabeledPoint(x, y, calcLabelAnn(X)));
		}
	}
}

int SpiralPointsLayer::calcLabelAnn(double* X) {
	double arrScore[_nClass];
	calcScoreAnn(X, arrScore);
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

void SpiralPointsLayer::calcScoreAnn(const double* X, double* arrScore) {
	double* pHiddenLayer = new double[_nHidden];
	for (size_t i = 0; i < _nHidden; i++)
	{
		pHiddenLayer[i] = _pB1->GetValue(0, i);
		for (size_t j = 0; j < _nD; j++)
		{
			pHiddenLayer[i] += X[j] * _pW1->GetValue(j, i);
		}
		if (pHiddenLayer[i] < 0) pHiddenLayer[i] = 0;
	}

	for (size_t i = 0; i < _nClass; i++)
	{
		arrScore[i] = _pB2->GetValue(0, i);
		for (size_t j = 0; j < _nHidden; j++)
		{
			arrScore[i] += pHiddenLayer[j] * _pW2->GetValue(j, i);
		}
	}

	delete[] pHiddenLayer;
}

void SpiralPointsLayer::trainAnn() {
	// train
	double dbStepSize = 1.0;
	double dbReg = 0.003;
	// set step to 1 to show the training procedure
	int nEpochs = 10000;
	for (size_t i = 0; i < nEpochs; i++)
	{
		trainStepAnn(dbStepSize, dbReg);
	}
}

void SpiralPointsLayer::trainStepAnn(double dbStepSize, double dbReg) {
	MyMatrix inputT(_pInput, true);

	// 1.evaluate class scores
	MyMatrix score(_nPoints, _nClass);
	evaluateScoreAnn(&score);

	// 2.compute the class probabilities
	double arrExpScores[_nPoints][_nClass];
	double arrExpScoreSum[_nPoints];
	for (size_t i = 0; i < _nPoints; i++)
	{
		arrExpScoreSum[i] = 0;
		for (size_t j = 0; j < _nClass; j++)
		{
			arrExpScores[i][j] = exp(score.GetValue(i,j));
			arrExpScoreSum[i] += arrExpScores[i][j];
		}
	}
	double arrProb[_nPoints][_nClass];
	for (size_t i = 0; i < _nPoints; i++)
	{
		for (size_t j = 0; j < _nClass; j++)
		{
			arrProb[i][j] = arrExpScores[i][j] / arrExpScoreSum[i];
		}
	}

	// 3.compute the loss: average cross-entropy loss and regularization
	double arrCorrectLogProb[_nPoints];
	double dbDataLoss = 0;
	for (size_t i = 0; i < _nPoints; i++)
	{
		arrCorrectLogProb[i] = -log(arrProb[i][_vecPoints[i]._nLabel]);
		dbDataLoss += arrCorrectLogProb[i];
	}
	dbDataLoss /= _nPoints;
	double dbRegLoss = 0.5*dbReg*_pW1->Norm2() + 0.5*dbReg*_pW2->Norm2();
	double dbLoss = dbRegLoss + dbDataLoss;
	cout << "Loss:\t" << dbLoss << "\t";

	// 4.compute the  gradient on scores
	MyMatrix dScore(_nPoints, _nClass);
	double dbCorrectProbs = 0;
	for (size_t j = 0; j < _nPoints; j++)
	{
		for (size_t k = 0; k < _nClass; k++)
		{
			if (k == _vecPoints[j]._nLabel)
			{
				dScore.SetValue(j, k, (arrProb[j][k] - 1) / _nPoints);
			}
			else {
				dScore.SetValue(j, k, (arrProb[j][k]) / _nPoints);
			}
		}
	}

	// 5.backpropagate the gradient to the parameters (W,b)
	// first backprop into parameters W2 and b2
	MyMatrix hiddenT(_pHidden, true);

	MyMatrix dW2(&hiddenT, &dScore);
	MyMatrix dB2(1, _nClass);
	for (size_t i = 0; i < _nClass; i++)
	{
		double dbB = 0;
		for (size_t j = 0; j < _nPoints; j++)
		{
			dbB += dScore.GetValue(j, i);
		}
		dB2.SetValue(0, i, dbB);
	}
	MyMatrix W2T(_pW2, true);
	MyMatrix dHidden(&dScore, &W2T);
//	dHidden.TrimNegative();
	for (size_t i = 0; i < _nPoints; i++)
	{
		for (size_t j = 0; j < _nClass; j++) {
			if (_pHidden->GetValue(i, j) < 0) dHidden.SetValue(i, j, 0);
		}
	}
	MyMatrix dW1(&inputT, &dHidden);
	MyMatrix dB1(1, _nHidden);
	for (size_t i = 0; i < _nHidden; i++)
	{
		double dbB = 0;
		for (size_t j = 0; j < _nPoints; j++)
		{
			dbB += dHidden.GetValue(j, i);
		}
		dB1.SetValue(0, i, dbB);
	}

	// 6.regularization gradient
	for (size_t j = 0; j < _nD; j++) {
		for (size_t k = 0; k < _nHidden; k++) {
			dW1.SetValue(j, k, dW1.GetValue(j, k) + dbReg*_pW1->GetValue(j, k));
		}
	}	
	for (size_t j = 0; j < _nHidden; j++) {
		for (size_t k = 0; k < _nClass; k++) {
			dW2.SetValue(j, k, dW2.GetValue(j, k) + dbReg*_pW2->GetValue(j, k));
		}
	}


	// 7.perform parameter update

	_pW1->Linear(&dW1, -dbStepSize);
	_pB1->Linear(&dB1, -dbStepSize);
	_pW2->Linear(&dW2, -dbStepSize);
	_pB2->Linear(&dB2, -dbStepSize);

	// 8.calculate score and accuracy
	int nPredicted = 0;
	for each (LabeledPoint pt in _vecPoints)
	{
		if (calcLabelAnn(pt._arrCoord) == pt._nLabel) nPredicted++;
	}
	cout << "Accuracy:\t" << nPredicted / (double)(_nPoints) << endl;;
}

void SpiralPointsLayer::evaluateScoreAnn(MyMatrix* pScore) {
	MyMatrix input(_nPoints, _nD);
	for (size_t i = 0; i < _nPoints; i++)
	{
		for (size_t j = 0; j < _nD; j++)
		{
			input.SetValue(i, j, _vecPoints[i]._arrCoord[j]);
		}
	}
	_pHidden->Formula(&input, _pW1, _pB1);
	pScore->Formula(_pHidden, _pW2, _pB2);
}