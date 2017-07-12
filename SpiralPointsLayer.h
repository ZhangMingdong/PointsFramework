#pragma once
#include "ILayer.h"
#include <vector>
#include "MyMatrix.h"

// point with label
class LabeledPoint
{
public:
	LabeledPoint() {};
	LabeledPoint(double x, double y, int nLabel) :_nLabel(nLabel) {
		_arrCoord[0] = x;
		_arrCoord[1] = y;
	}
	double _arrCoord[2];
	int _nLabel;
};

/*
	Layer to show the spiral points and the softmax and ann classifier
*/
class SpiralPointsLayer : public ILayer
{
public:
	SpiralPointsLayer();
	virtual ~SpiralPointsLayer();
	virtual void Draw();
	virtual void Clear();
	virtual void UpdateLayer();
private:
	// number of points of each class
	static const int _nPointPerClass=100;
	// number of classes
	static const int _nClass = 3;
	// dimension
	static const int _nD = 2;
	// number of training points
	static const int _nPoints = _nPointPerClass*_nClass;
	// points
	std::vector<LabeledPoint> _vecPoints;
	// W 
	MyMatrix* _pW;
	// b
	MyMatrix* _pB;
	// points and labels of the result
	std::vector<LabeledPoint> _vecResultPt;

	// for ann
	// number of hidden layers
	static const int _nHidden=100;
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


private:
	// generate the spiral points
	void generatePoints();
	// initialize the parameters
	void initializeParams();
	// calculate the label of point
	int calcLabel(double* X);
	// calculate score
	void calcScore(const double* X, double* arrScore);
	// evaluate scores of all the instance
	void evaluateScore(double (*arrScores)[_nClass]);
	// train
	void train();
	// an epoch of the training
	void trainStep(double dbStepSize,double dbReg);
	// show the classifier
	void showClassifier();
	// for ann
	// show the classifier
	void showClassifierAnn();

	// initialize the parameters
	void initializeParamsAnn();

	// calculate the label of point
	int calcLabelAnn(double* X);
	// calculate score
	void calcScoreAnn(const double* X, double* arrScore);
	// train
	void trainAnn();
	// an epoch of the training
	void trainStepAnn(double dbStepSize, double dbReg);
	// evaluate scores of all the instance
	void evaluateScoreAnn(double(*arrScores)[_nClass]);

};

