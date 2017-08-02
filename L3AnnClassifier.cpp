#include "L3AnnClassifier.h"
#include <math.h>
#include <iostream>

using namespace std;


L3AnnClassifier::L3AnnClassifier(int nHidden):_pW1(0)
, _pB1(0)
, _pW2(0)
, _pB2(0)
, _pW3(0)
, _pB3(0)
, _pHidden1(0)
, _pHidden2(0)
, _nHidden1(nHidden)
, _nHidden2(nHidden)
{

}


void L3AnnClassifier::initializeImp() {
	_pW1 = new MyMatrix(_nD, _nHidden1);
	_pB1 = new MyMatrix(1, _nHidden1);

	_pW2 = new MyMatrix(_nHidden1, _nHidden2);
	_pB2 = new MyMatrix(1, _nHidden2);

	_pW3 = new MyMatrix(_nHidden2, _nClass);
	_pB3 = new MyMatrix(1, _nClass);

	_pHidden1 = new MyMatrix(_nPoints, _nHidden1);
	_pHidden2 = new MyMatrix(_nPoints, _nHidden2);

	_pB1->InitZero();
	_pW1->InitRandom(0.01);
	_pB2->InitZero();
	_pW2->InitRandom(0.01);
	_pB3->InitZero();
	_pW3->InitRandom(0.01);
}

L3AnnClassifier::~L3AnnClassifier()
{
	if (_pW1) delete _pW1;
	if (_pB1) delete _pB1;
	if (_pW2) delete _pW2;
	if (_pB2) delete _pB2;
	if (_pW3) delete _pW3;
	if (_pB3) delete _pB3;
	if (_pHidden1) delete _pHidden1;
	if (_pHidden2) delete _pHidden2;
}

void L3AnnClassifier::Train(const MyMatrix* pInput, const int* pLabel) {
	_pI = pInput;
	_arrLabel = pLabel;
	// train
	double dbStepSize = 4.0;
	double dbReg = 0.00001;
	// set step to 1 to show the training procedure
	int nEpochs = 20;
	for (size_t i = 0; i < nEpochs; i++)
	{
		trainStep(dbStepSize, dbReg);
	}
}

void L3AnnClassifier::trainStep(double dbStepSize, double dbReg) {
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
		double dbCorrectLogProb = -log(prob.GetValue(i, _arrLabel[i]));// -log(arrProb[i][_arrLabel[i]]);
		dbDataLoss += dbCorrectLogProb;
	}
	dbDataLoss /= _nPoints;
	double dbRegLoss = 0.5*dbReg*(_pW1->Norm2()+_pW2->Norm2()+ _pW3->Norm2());
	double dbLoss = dbRegLoss + dbDataLoss;
	cout << "Loss:\t" << dbLoss << "\t";

	// 4.compute the  gradient on scores
	MyMatrix dScore(&prob);
	for (size_t j = 0; j < _nPoints; j++)
	{
		dScore.SetValue(j, _arrLabel[j], (prob.GetValue(j, _arrLabel[j]) - 1));
	}
	dScore.Multi(1.0 / _nPoints);

	// 5.backpropagate the gradient to the parameters
	// first backprop into parameters w3 and b3
	MyMatrix hiddenT2(_pHidden2, true);
	MyMatrix dW3(&hiddenT2, &dScore);
	MyMatrix dB3;
	dScore.Sum(&dB3, 0);

	// next backprop into hidden layer 2
	MyMatrix W3T(_pW3, true);
	MyMatrix dHidden2(&dScore, &W3T);
	// backprop the ReLU non-linearity
	for (size_t i = 0; i < _nPoints; i++)
	{
		for (size_t j = 0; j < _nHidden2; j++)
		{
			if (_pHidden2->GetValue(i, j) <= 0)
			{
				dHidden2.SetValue(i, j, 0);
			}
		}
	}

	MyMatrix hiddenT1(_pHidden1, true);
	MyMatrix dW2(&hiddenT1, &dHidden2);
	MyMatrix dB2;
	dHidden2.Sum(&dB2, 0);

	// next backprop into hidden layer
	MyMatrix W2T(_pW2, true);
	MyMatrix dHidden1(&dScore, &W2T);
	// backprop the ReLU non-linearity
	for (size_t i = 0; i < _nPoints; i++)
	{
		for (size_t j = 0; j < _nHidden1; j++)
		{
			if (_pHidden1->GetValue(i, j) <= 0)
			{
				dHidden1.SetValue(i, j, 0);
			}
		}
	}

	MyMatrix inputT(_pI, true);
	MyMatrix dW1(&inputT, &dHidden1);
	MyMatrix dB1;
	dHidden1.Sum(&dB1, 0);



	// 6.regularization gradient
	for (size_t j = 0; j < _nHidden2; j++) {
		for (size_t k = 0; k < _nClass; k++) {
			dW3.SetValue(j, k, dW2.GetValue(j, k) + dbReg*_pW3->GetValue(j, k));
		}
	}
	for (size_t j = 0; j < _nHidden1; j++) {
		for (size_t k = 0; k < _nHidden2; k++) {
			dW2.SetValue(j, k, dW2.GetValue(j, k) + dbReg*_pW2->GetValue(j, k));
		}
	}	
	for (size_t j = 0; j < _nD; j++) {
		for (size_t k = 0; k < _nHidden1; k++) {
			dW1.SetValue(j, k, dW1.GetValue(j, k) + dbReg*_pW1->GetValue(j, k));
		}
	}

	// 7.perform parameter update
	_pW1->Linear(&dW1, -dbStepSize);
	_pB1->Linear(&dB1, -dbStepSize);
	_pW2->Linear(&dW2, -dbStepSize);
	_pB2->Linear(&dB2, -dbStepSize);
	_pW3->Linear(&dW3, -dbStepSize);
	_pB3->Linear(&dB3, -dbStepSize);

	// 8.calculate score and accuracy
	int nPredicted = 0;
	for (size_t i = 0; i < _nPoints; i++)
	{
		if (CalcLabel(_pI->GetRow(i)) == _arrLabel[i]) nPredicted++;
	}
	cout << "Accuracy:\t" << nPredicted / (double)(_nPoints) << endl;;
}


int L3AnnClassifier::CalcLabel(const double* X) {
	MyMatrix score(1, _nClass);
	MyMatrix input(1, _nD);
	for (size_t i = 0; i < _nD; i++)
	{
		input.SetValue(0, i, X[i]);
	}
	MyMatrix hidden1(1, _nHidden1);
	hidden1.Formula(&input, _pW1, _pB1);
	hidden1.TrimNegative();


	MyMatrix hidden2(1, _nHidden2);
	hidden2.Formula(&hidden1, _pW2, _pB2);
	hidden2.TrimNegative();

	score.Formula(&hidden2, _pW3, _pB3);

	int nResult = 0;
	double dbMaxScore = score.GetValue(0, 0);
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

void L3AnnClassifier::evaluateScore(MyMatrix* pScores) {
	_pHidden1->Formula(_pI, _pW1, _pB1);
	_pHidden1->TrimNegative();

	_pHidden2->Formula(_pHidden1, _pW2, _pB2);
	_pHidden2->TrimNegative();

	pScores->Formula(_pHidden2, _pW3, _pB3);
}

void L3AnnClassifier::GetHiddenLayer(int nIndex, double* _arrBuf) {
	for (size_t i = 0; i < _nD; i++)
	{
		_arrBuf[i] = _pW1->GetValue(i,nIndex);
	}
}