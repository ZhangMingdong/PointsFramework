#pragma once
#include "ILayer.h"
#include <vector>
#include "MyMatrix.h"

#include "IMyClassifier.h"

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


	// input layer
	MyMatrix* _pInput;
	// output layer
	MyMatrix* _pOutput;

	// classifier
	IMyClassifier* _pClassifier;
	// the labels
	int* _arrLabels;
private:
	// generate points and set them to the form of input data
	void generatePoints();

	// generate points of different circle
	void generateCircularPoints();

	// generate the spiral points
	void generateSpiralPoints();

	// train
	void train();

	// show the classifier
	void showClassifier();

};

