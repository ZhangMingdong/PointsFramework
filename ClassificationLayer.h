#pragma once
#include "ILayer.h"
#include <vector>
#include "MyMatrix.h"

#include "IMyClassifier.h"
class BPNeuralNetwork;
class LabeledVector;
class TextureRenderer;

/*
	Layer for classification
	Mingdong
	2019/08/09
*/
class ClassificationLayer : public ILayer
{
public:
	ClassificationLayer();
	virtual ~ClassificationLayer();
public:
	virtual void Draw();
	virtual void Clear();
	virtual void Initialize();
	virtual void UpdateLayer();
protected:
	// generate points and set them to the form of input data
	virtual void generatePoints() = 0;
	virtual void preparePointsForClassification();
protected:
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
	std::vector<ColoredPoint> _vecResultPtColored;

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

	// train
	void train();

	// show the classifier
	void showClassifier();

	// generate points of the hidden layer
	void generateHiddenLayerPoints();
protected:
	// the texture renderer
	TextureRenderer* _pTRenderer = NULL;
};

