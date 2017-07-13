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

void SoftMaxClassifier::trainStep(double dbStepSize, double dbReg) {

	MyMatrix inputT(_pI, true);

	// 1.evaluate class scores
	MyMatrix scores(_nPoints, _nClass);
	evaluateScore(&scores);

	// 2.compute the class probabilities
	MyMatrix expScores(&scores, exp);
	MyMatrix expScoreSum;
	expScores.Sum(&expScoreSum, 1);
	MyMatrix prob;
	expScores.Div(&prob, &expScoreSum);

	// 3.compute the loss: average cross-entropy loss and regularization
	double dbDataLoss = 0;
	for (size_t i = 0; i < _nPoints; i++)
	{
		double dbCorrectLogProb = -log(prob.GetValue(i,_arrLabel[i]));// -log(arrProb[i][_arrLabel[i]]);
		dbDataLoss += dbCorrectLogProb;
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
				dScore.SetValue(j, k, (prob.GetValue(j,k) - 1) / _nPoints);
			}
			else {
				dScore.SetValue(j, k, (prob.GetValue(j, k)) / _nPoints);
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
	for (size_t i = 0; i < _nPoints; i++)
	{
		if (CalcLabel(_pI->GetRow(i)) == _arrLabel[i]) nPredicted++;
	}
	cout << "Accuracy:\t" << nPredicted / (double)(_nPoints) << endl;;
}


int SoftMaxClassifier::CalcLabel(const double* X) {
	MyMatrix score(1, _nClass);
	MyMatrix input(1, _nD);
	for (size_t i = 0; i < _nD; i++)
	{
		input.SetValue(0, i, X[i]);
	}
	score.Formula(&input, _pW, _pB);

	int nResult = 0;
	double dbMaxScore = score.GetValue(0,0);
	for (size_t i = 1; i < _nClass; i++)
	{
		double dbValue = score.GetValue(0, i);
		if (dbValue > dbMaxScore) {
			nResult = i;
			dbMaxScore = dbValue;
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