#pragma once
#include "MyMatrix.h"
/*
	SoftMaxClassifier
*/
class SoftMaxClassifier
{
public:
	SoftMaxClassifier(int nPoints,int nD,int nClass);
	virtual ~SoftMaxClassifier();
private:
	// W 
	MyMatrix* _pW;
	// b
	MyMatrix* _pB;
	// input
	const MyMatrix* _pI;
	// output
	const int* _arrLabel;
	// number of classes
	int _nClass;
	// dimension
	int _nD;
	// number of training points
	int _nPoints;
public:
	// training
	void Train(const MyMatrix* pInput, const int* pLabel);
	// calculate the label of point
	int calcLabel(double* X);
private:
	// an epoch of the training
	void trainStep(double dbStepSize, double dbReg);
	// calculate score
	void calcScore(const double* X, double* arrScore);
	// evaluate scores of all the instance
	void evaluateScore(MyMatrix* pScores);
};

