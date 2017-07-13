#pragma once
#include "MyMatrix.h"
/*
	Artificial Neural Network classifier
*/
class AnnClassifier
{
public:
	AnnClassifier(int nPoints, int nD, int nClass);
	virtual ~AnnClassifier();	
private:
	// W1 
	MyMatrix* _pW1;
	// b1
	MyMatrix* _pB1;
	// W2 
	MyMatrix* _pW2;
	// b2
	MyMatrix* _pB2;
	// hidden layer
	MyMatrix* _pHidden;
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
	// number of hidden layers
	static const int _nHidden = 100;
public:
	// training
	void Train(const MyMatrix* pInput, const int* pLabel);
	// calculate the label of point
	int CalcLabel(const double* X);
private:
	// an epoch of the training
	void trainStep(double dbStepSize, double dbReg);
	// evaluate scores of all the instance
	void evaluateScore(MyMatrix* pScores);
};

