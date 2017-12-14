#include "SpiralPointsLayer.h"
#include <QGLWidget>
#include <gl/GLU.h>
#include <iostream>

#include "BPNeuralNetwork.h"
#include "TextureRenderer.h"

#define TRAIN_ANN
//#define BP_ANN

using namespace std;

// number of hidden layers
int g_nHiddenLayer = 100;

SpiralPointsLayer::SpiralPointsLayer():_arrLabels(0),_pBPNN(NULL)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	generatePoints();

#ifdef BP_ANN
	_pVector = new LabeledVector(2);
	_pVector->SetK(3);
	for each (LabeledPoint pt in _vecPoints)
	{
		LabeledVector* pV = new LabeledVector(2);
		pV->SetK(3);
		pV->SetData(0, pt._arrCoord[0]);
		pV->SetData(1, pt._arrCoord[1]);
		pV->SetLabel(pt._nLabel);
		_vecTrainingSet.push_back(pV);
	}

	int nHiddenLayer = 10;
	_pBPNN = BPNeuralNetwork::Create(2, nHiddenLayer, _nClass);
#else
	#ifdef TRAIN_ANN
		_pClassifier = IMyClassifier::CreateClassifier(IMyClassifier::Ann, _nPoints, _nD, _nClass, g_nHiddenLayer);
	#else
		_pClassifier = IMyClassifier::CreateClassifier(IMyClassifier::SoftMax, _nPoints, _nD, _nClass);
	#endif
#endif
//	showClassifier();


	// the classifier will be changed when pushing the update button
}

SpiralPointsLayer::~SpiralPointsLayer()
{
	delete _pInput;
	delete _pOutput;

	if(_pClassifier) delete _pClassifier;

	if (_arrLabels) delete[] _arrLabels;

	if (_pBPNN) delete _pBPNN;
	if (_pVector) delete _pVector;
	if (_pTRenderer) delete[] _pTRenderer;
}

void SpiralPointsLayer::generatePoints() {
//	generateSpiralPoints();
//	generateCircularPoints();
//	generateMultiNormalPoints();
//	generateRectangularGridPoints();
	generateCircularGridPoints();

	_nPoints = _vecPoints.size();

	_pInput = new MyMatrix(_nPoints, _nD);
	_pOutput = new MyMatrix(_nPoints, 1);

	for (size_t i = 0; i < _nPoints; i++)
	{
		for (size_t j = 0; j < _nD; j++)
		{
			_pInput->SetValue(i, j, _vecPoints[i]._arrCoord[j]);
		}
	}
	_arrLabels = new int[_nPoints];
	for (size_t i = 0; i < _nPoints; i++)
	{
		_arrLabels[i] = _vecPoints[i]._nLabel;

	}

}

void SpiralPointsLayer::generateSpiralPoints() {
	double dbBiasX = .1;
	double dbBiasY = .2;
	_vecPoints.clear();
	// generate spiral points
	for (size_t i = 0; i < _nClass; i++)
	{
		// for each class
		for (size_t j = 0; j < _nPointPerClass; j++)
		{
			// add each point
			double r = j / (double)(_nPointPerClass - 1);
			// in the web the delt is 0.2, but I use 0.002. No idea why
			double t = i * 4 + j*4.0 / (_nPointPerClass - 1) + rand() / (double)RAND_MAX*_nPointPerClass*0.002;

			_vecPoints.push_back(LabeledPoint(dbBiasX + r*sin(t), dbBiasY + r*cos(t), i));
		}
	}
}

void SpiralPointsLayer::generateMultiNormalPoints() {
	
	std::vector<DPoint3> pts[3];
	double arrMX[3] = { -1,1,0 };
	double arrMY[3] = { 1,1,-1 };
	double arrDX[3] = { .5,.5,.5 };
	double arrDY[3] = { .5,.5,.5 };
	for (size_t i = 0; i < 3; i++)
	{
		GenerateNormalPoints(pts[i], _nPointPerClass, arrMX[i], arrMY[i], arrDX[i], arrDY[i]);
		for each (DPoint3 pt in pts[i])
		{
			_vecPoints.push_back(LabeledPoint(pt.x,pt.y,i));
		}
	}
}

void SpiralPointsLayer::generateCircularPoints() {
	double dbBiasX = .1;
	double dbBiasY = .2;
	_vecPoints.clear();
	// generate spiral points
	for (size_t i = 0; i < _nClass; i++)
	{
		// for each class
		for (size_t j = 0; j < _nPointPerClass; j++)
		{
			// add each point
			// the radius
			double r = (i + 1) / 5.0 + rand() / (double)RAND_MAX*0.02;
			// in the web the delt is 0.2, but I use 0.002. No idea why
			double t = 2 * 3.14159*j / _nPointPerClass;

			_vecPoints.push_back(LabeledPoint(dbBiasX + r*sin(t), dbBiasY + r*cos(t), i));
		}
		// add this line to bias the circles
		dbBiasX += .5;
	}
}

void SpiralPointsLayer::generateRectangularGridPoints() {
	double xMin =  - 1;
	double xMax =  + 1;
	double yMin =  - 1;
	double yMax =  + 1;
	double h = 0.1;
	for (double x = xMin; x <= xMax; x += h)
	{
		for (double y = yMin; y <= yMax; y += h)
		{
			double X[_nD] = { x,y };
			int nLabel = 0;
			if (abs(x) < .5&&abs(y) < .5) {
				if (x > y) nLabel = 1;
				else nLabel = 2;
			}
			_vecPoints.push_back(LabeledPoint(x, y, nLabel));
		}
	}
}

void SpiralPointsLayer::generateCircularGridPoints() {
	double xMin = -1;
	double xMax = +1;
	double yMin = -1;
	double yMax = +1;
	double h = 0.1;
	for (double x = xMin; x <= xMax; x += h)
	{
		for (double y = yMin; y <= yMax; y += h)
		{
			double X[_nD] = { x,y };
			int nLabel = 0;
			if (x*x+y*y<0.64) {
				nLabel = 1;
				if (x*x + y*y < 0.16)
					nLabel = 2;
			}
			_vecPoints.push_back(LabeledPoint(x, y, nLabel));
		}
	}
}

void SpiralPointsLayer::Draw() {
	// draw the spiral points
	double colors[3][3] = {
		{1,0,0}
		,{0,1,0}
		,{0,0,1}
	};
	glPointSize(2.0f);
	glBegin(GL_POINTS);
	for each (LabeledPoint pt in _vecPoints)
	{
		glColor3dv(colors[pt._nLabel]);
		glVertex3d(pt._arrCoord[0], pt._arrCoord[1], 0);
	}
	glEnd();

	// draw background
	double dbOpacity = 0.5;
	double Rcolors[3][4] = {
		{ 1,0,0,dbOpacity }
		,{ 0,1,0,dbOpacity }
		,{ 0,0,1,dbOpacity }
	};
	glPointSize(2.0f);


	float _fLeft = -_pSetting->_dbRadius;
	float _fRight = _pSetting->_dbRadius;
	float _fBottom = -_pSetting->_dbRadius;
	float _fTop = _pSetting->_dbRadius;
	if (_pTRenderer)
		_pTRenderer->Draw(_fLeft, _fRight, _fTop, _fBottom);
	/*
	bool bShowMixedColor = true;
	if (bShowMixedColor) {

		glBegin(GL_POINTS);
		for (ColoredPoint pt:_vecResultPtColored)
		{
			glColor3dv(pt._arrColor);
			glVertex3d(pt._arrCoord[0], pt._arrCoord[1], 0);
		}
		glEnd();
	}
	else {

		glBegin(GL_POINTS);
		for (size_t i = 0, length = _vecResultPt.size(); i < length; i++)
		{
			glColor4dv(Rcolors[_vecResultPt[i]._nLabel]);
			glVertex3d(_vecResultPt[i]._arrCoord[0], _vecResultPt[i]._arrCoord[1], 0);
		}
		glEnd();
	}
	*/
	// draw hidden layer points
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor4d(0, 1.0, 1.0, .5);
	for each (LabeledPoint pt in _vecHiddenlayerPt)
	{
		glVertex3d(pt._arrCoord[0], pt._arrCoord[1], 0);
	}
	glEnd();
}

void SpiralPointsLayer::Clear() {

}

void SpiralPointsLayer::train() {
#ifdef BP_ANN
	int epochs = 10000;
	double dbLearningRate = .01;
	double dbMomentum = 0.1;
	_pBPNN->TrainNet(epochs, _vecTrainingSet, dbLearningRate, dbMomentum);

#else
	_pClassifier->Train(_pInput, _arrLabels);
#endif
}

void SpiralPointsLayer::showClassifier() {
	_pTRenderer = new TextureRenderer(_pSetting->_nResultLen, _pSetting->_nResultLen);
	// 2.generate result
	double dbStep = _pSetting->_dbRadius * 2 / _pSetting->_nResultLen;
	for (size_t i = 0; i < _pSetting->_nResultLen; i++)
	{
		for (size_t j = 0; j < _pSetting->_nResultLen; j++)
		{
			double x = -_pSetting->_dbRadius + dbStep*j;
			double y = -_pSetting->_dbRadius + dbStep*i;

			int nIndex = i*_pSetting->_nResultLen + j;


			double X[_nD] = { x,y };

			bool bMixedColor = true;
			if (bMixedColor) {
				double arrScores[3];
				_pClassifier->CalcScores(X, arrScores);
				GLubyte bufData[4] = { (GLubyte)(arrScores[0] * 200)
					, (GLubyte)(arrScores[1] * 200)
					, (GLubyte)(arrScores[2] * 200)
					, (GLubyte)255 };
				_pTRenderer->SetTextureData(nIndex, bufData);
			}
			else {
				int nLabel = _pClassifier->CalcLabel(X);
				GLubyte bufData[4] = { (GLubyte)(nLabel == 0 ? 255 : 0)
					, (GLubyte)(nLabel == 1 ? 255 : 0)
					, (GLubyte)(nLabel == 2 ? 255 : 0)
					, (GLubyte)255 };
				_pTRenderer->SetTextureData(nIndex, bufData);
			}

		}
	}
	_pTRenderer->GenerateTexture();

	/*
	_vecResultPt.clear();
	_vecResultPtColored.clear();
	// generate the result plot
	double xMin = _vecPoints[0]._arrCoord[0];
	double xMax = _vecPoints[0]._arrCoord[0];
	double yMin = _vecPoints[0]._arrCoord[1];
	double yMax = _vecPoints[0]._arrCoord[1];
	for each (LabeledPoint pt in _vecPoints)
	{
		if (pt._arrCoord[0] < xMin) xMin = pt._arrCoord[0];
		if (pt._arrCoord[0] > xMax) xMax = pt._arrCoord[0];
		if (pt._arrCoord[1] < yMin) yMin = pt._arrCoord[1];
		if (pt._arrCoord[1] > yMax) yMax = pt._arrCoord[1];
	}
	xMin = xMin - 1;
	xMax = xMax + 1;
	yMin = yMin - 1;
	yMax = yMax + 1;
	double h = 0.01;
	for (double x = xMin; x < xMax; x += h)
	{
		for (double y = yMin; y < yMax; y += h)
		{
			double X[_nD] = { x,y };
#ifdef BP_ANN
			_pVector->SetData(0, x);
			_pVector->SetData(1, y);
			_pVector->SetLabel(0);
			_pBPNN->LoadInputData(_pVector);
			_pBPNN->FeedForward();
			_vecResultPt.push_back(LabeledPoint(x, y, _pBPNN->CalculateLabel()-1));
#else
			_vecResultPt.push_back(LabeledPoint(x, y, _pClassifier->CalcLabel(X)));
			double arrScores[3];
			_pClassifier->CalcScores(X, arrScores);
			_vecResultPtColored.push_back(ColoredPoint(x, y,arrScores[0],arrScores[1],arrScores[2]));

#endif
		}
	}
	*/
}

void SpiralPointsLayer::UpdateLayer() {
	// train the parameter
	train();
	// reload the visualization of the classifier
	showClassifier();

//	generateHiddenLayerPoints();
}

void SpiralPointsLayer::generateHiddenLayerPoints() {
	for (size_t i = 0; i < g_nHiddenLayer; i++)
	{
		LabeledPoint pt(0, 0, 0);
		_pClassifier->GetHiddenLayer(i, pt._arrCoord);
		_vecHiddenlayerPt.push_back(pt);

	}
}