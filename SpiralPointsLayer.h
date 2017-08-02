#pragma once
#include "ILayer.h"
#include <vector>
#include "MyMatrix.h"

#include "IMyClassifier.h"
class BPNeuralNetwork;
class LabeledVector;


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
	static const int _nPointPerClass=300;
	// number of classes
	static const int _nClass = 3;
	// dimension
	static const int _nD = 2;
	// number of training points
	int _nPoints = _nPointPerClass*_nClass;
	// points
	std::vector<LabeledPoint> _vecPoints;
	// points and labels of the result
	std::vector<LabeledPoint> _vecResultPt;

	// points of hidden layers
	std::vector<LabeledPoint> _vecHiddenlayerPt;


	// input layer
	MyMatrix* _pInput;
	// output layer
	MyMatrix* _pOutput;

	// classifier
	IMyClassifier* _pClassifier=NULL;
	// the labels
	int* _arrLabels;

	// bpnn classifer
	BPNeuralNetwork* _pBPNN = NULL;
	// the vector
	LabeledVector* _pVector = NULL;
	std::vector<LabeledVector*> _vecTrainingSet;
private:
	// generate points and set them to the form of input data
	void generatePoints();

	// generate points of different circle
	void generateCircularPoints();

	// generate the spiral points
	void generateSpiralPoints();

	// generate points obey multi-normal distribution
	void generateMultiNormalPoints();

	// generate rectangular grid points
	void generateRectangularGridPoints();

	// generate circular grid points
	void generateCircularGridPoints();

	// train
	void train();

	// show the classifier
	void showClassifier();

	// generate points of the hidden layer
	void generateHiddenLayerPoints();

};

