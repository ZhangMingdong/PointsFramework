#include "SoftMaxClassifier.h"
#include <math.h>
#include <iostream>

using namespace std;


SoftMaxClassifier::SoftMaxClassifier(int nPoints, int nD, int nClass):_nPoints(nPoints)
,_nD(nD)
,_nClass(nClass)
{
	_pW = new MyMatrix(_nD, _nClass);
	_pB = new MyMatrix(1, _nClass);
	_pB->InitZero();
	_pW->InitRandom();
}


SoftMaxClassifier::~SoftMaxClassifier()
{
	delete _pW;
	delete _pB;
	delete _pI;
}

void SoftMaxClassifier::Train(const MyMatrix* pInput, const int* pLabel) {
	_pI = pInput;
	_arrLabel = pLabel;
	// train
	double dbStepSize = 1.0;
	double dbReg = 0.003;
	// set step to 1 to show the training procedure
	int nEpochs = 200;
	for (size_t i = 0; i < nEpochs; i++)
	{
		trainStep(dbStepSize, dbReg);
	}
}

// number of points of each class
const int g_nPointPerClass = 100;
// number of classes
const int g_nClass = 3;
// dimension
const int g_nD = 2;
// number of training points
const int g_nPoints = g_nPointPerClass*g_nClass;

void SoftMaxClassifier::trainStep(double dbStepSize, double dbReg) {

	MyMatrix inputT(_pI, true);

	// 1.evaluate class scores
	MyMatrix scores(_nPoints, _nClass);
	evaluateScore(&scores);

	// 2.compute the class probabilities
	double arrExpScores[g_nPoints][g_nClass];
	double arrExpScoreSum[g_nPoints];
	for (size_t i = 0; i < _nPoints; i++)
	{
		arrExpScoreSum[i] = 0;
		for (size_t j = 0; j < _nClass; j++)
		{
			arrExpScores[i][j] = exp(scores.GetValue(i, j));
			arrExpScoreSum[i] += arrExpScores[i][j];
		}
	}
	double arrProb[g_nPoints][g_nClass];
	for (size_t i = 0; i < _nPoints; i++)
	{
		for (size_t j = 0; j < _nClass; j++)
		{
			arrProb[i][j] = arrExpScores[i][j] / arrExpScoreSum[i];
		}
	}

	// 3.compute the loss: average cross-entropy loss and regularization
	double arrCorrectLogProb[g_nPoints];
	double dbDataLoss = 0;
	for (size_t i = 0; i < _nPoints; i++)
	{
		arrCorrectLogProb[i] = -log(arrProb[i][_arrLabel[i]]);
		dbDataLoss += arrCorrectLogProb[i];
	}
	dbDataLoss /= _nPoints;
	double dbRegLoss = 0.5*dbReg*_pW->Norm2();
	double dbLoss = dbRegLoss + dbDataLoss;
	cout << "Loss:\t" << dbLoss << "\t";

	// 4.compute the  gradient on scores
	MyMatrix dScore(_nPoints, _nClass);
	double dbCorrectProbs = 0;
	for (size_t j = 0; j < _nPoints; j++)
	{
		for (size_t k = 0; k < _nClass; k++)
		{
			if (k == _arrLabel[j])
			{
				dScore.SetValue(j, k, (arrProb[j][k] - 1) / _nPoints);
			}
			else {
				dScore.SetValue(j, k, (arrProb[j][k]) / _nPoints);
			}
		}
	}
	// 5.backpropagate the gradient to the parameters (W,b)
	MyMatrix dW(&inputT, &dScore);
	MyMatrix dB(1, _nClass);

	for (size_t j = 0; j < _nClass; j++)
	{
		double dbDB = 0;
		for (size_t k = 0; k < _nPoints; k++)
		{
			dbDB += dScore.GetValue(k, j);
		}
		dB.SetValue(0, j, dbDB);
	}

	// 6.regularization gradient
	for (size_t j = 0; j < _nD; j++) {
		for (size_t k = 0; k < _nClass; k++) {
			dW.SetValue(j, k, dW.GetValue(j, k) + dbReg*_pW->GetValue(j, k));
		}
	}
	// 7.perform parameter update
	_pW->Linear(&dW, -dbStepSize);
	_pB->Linear(&dB, -dbStepSize);

	// 8.calculate score and accuracy
	int nPredicted = 0;
//	for (size_t i = 0; i < _nPoints; i++)
//	{
//		if (calcLabel(pt._arrCoord) == pt._nLabel) nPredicted++;
//	}
	cout << "Accuracy:\t" << nPredicted / (double)(_nPoints) << endl;;
}


int SoftMaxClassifier::calcLabel(double* X) {
	double arrScore[g_nClass];
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

void SoftMaxClassifier::calcScore(const double* X, double* arrScore) {
	for (size_t i = 0; i < _nClass; i++)
	{
		arrScore[i] = _pB->GetValue(0, i);
		for (size_t j = 0; j < _nD; j++)
		{
			arrScore[i] += X[j] * _pW->GetValue(j, i);
		}
	}
}

void SoftMaxClassifier::evaluateScore(MyMatrix* pScores) {
	pScores->Formula(_pI, _pW, _pB);
}