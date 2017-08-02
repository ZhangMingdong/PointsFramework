#pragma once
#include"IMyClassifier.h"
/*
	3 layers Artificial Neural Network classifier
*/
class L3AnnClassifier:public IMyClassifier
{
public:
	L3AnnClassifier(int nHidden);
	virtual ~L3AnnClassifier();	
private:
	// W1 
	MyMatrix* _pW1;
	// b1
	MyMatrix* _pB1;
	// W2 
	MyMatrix* _pW2;
	// b2
	MyMatrix* _pB2;
	// W2 
	MyMatrix* _pW3;
	// b2
	MyMatrix* _pB3;
	// hidden layer
	MyMatrix* _pHidden1;
	MyMatrix* _pHidden2;
	// number of hidden layers
	int _nHidden1 = 100;
	int _nHidden2 = 100;
protected:
	// initialized the classifer
	void initializeImp();
public:
	// training
	void Train(const MyMatrix* pInput, const int* pLabel);
	// calculate the label of point
	int CalcLabel(const double* X);
	// get the nIndex column of the hidden layer
	virtual void GetHiddenLayer(int nIndex, double* _arrBuf);
private:
	// an epoch of the training
	void trainStep(double dbStepSize, double dbReg);
	// evaluate scores of all the instance
	void evaluateScore(MyMatrix* pScores);
};

