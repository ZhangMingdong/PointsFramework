#pragma once
#include "ILayer.h"
#include <vector>
#include "MyMatrix.h"

#include "SoftMaxClassifier.h"
#include "AnnClassifier.h"

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
	// points and labels of the result
	std::vector<LabeledPoint> _vecResultPt;

	// for ann

	// input layer
	MyMatrix* _pInput;
	// output layer
	MyMatrix* _pOutput;

	// softmax classifier
	SoftMaxClassifier* _pSoftMax;
	// ann classifier
	AnnClassifier* _pAnn;
	// the labels
	int* _arrLabels;
private:
	// generate the spiral points
	void generatePoints();
	// initialize the parameters
	void initializeParams();

	// train
	void train();
	// an epoch of the training
//	void trainStep(double dbStepSize,double dbReg);
	// show the classifier
	void showClassifier();
	// for ann
	// show the classifier
	void showClassifierAnn();

	// initialize the parameters
	void initializeParamsAnn();

	// calculate the label of point
	int calcLabelAnn(double* X);
	// train
	void trainAnn();

	/*
	// calculate score
	void calcScoreAnn(const double* X, double* arrScore);
	// an epoch of the training
	void trainStepAnn(double dbStepSize, double dbReg);
	// evaluate scores of all the instance
	void evaluateScoreAnn(MyMatrix* pScore);
	*/
};

