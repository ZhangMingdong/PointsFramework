#include "ClassificationLayer.h"
#include <QGLWidget>
#include <gl/GLU.h>
#include <iostream>

#include "BPNeuralNetwork.h"
#include "TextureRenderer.h"

#define TRAIN_ANN


using namespace std;

// number of hidden layers
int g_nHiddenLayer = 100;

void ClassificationLayer::Initialize() {	
	generatePoints();
	_nPoints = _vecPoints.size();
	preparePointsForClassification();
#ifdef TRAIN_ANN
	_pClassifier = IMyClassifier::CreateClassifier(IMyClassifier::Ann, _nPoints, _nD, _nClass, g_nHiddenLayer);
#else
	_pClassifier = IMyClassifier::CreateClassifier(IMyClassifier::SoftMax, _nPoints, _nD, _nClass);
#endif
}
ClassificationLayer::ClassificationLayer():_arrLabels(0),_pBPNN(NULL)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);





//	showClassifier();


	// the classifier will be changed when pushing the update button
}

ClassificationLayer::~ClassificationLayer()
{
	delete _pInput;
	delete _pOutput;

	if(_pClassifier) delete _pClassifier;

	if (_arrLabels) delete[] _arrLabels;

	if (_pBPNN) delete _pBPNN;
	if (_pVector) delete _pVector;
	if (_pTRenderer) delete[] _pTRenderer;
}

void ClassificationLayer::Draw() {

	// draw the spiral points
	double colors[3][3] = {
		{1,0,0}
		,{0,1,0}
		,{0,0,1}
	};
	//glPointSize(2.0f);
	glPointSize(_pSetting->_dbPointSize);
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

	// draw hidden layer points
	// ???
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor4d(0, 1.0, 1.0, .5);
	for each (LabeledPoint pt in _vecHiddenlayerPt)
	{
		glVertex3d(pt._arrCoord[0], pt._arrCoord[1], 0);
	}
	glEnd();
}

void ClassificationLayer::Clear() {

}

void ClassificationLayer::train() {

	_pClassifier->Train(_pInput, _arrLabels);

}

void ClassificationLayer::showClassifier() {
	// 1.create new instance
	if (_pTRenderer) delete _pTRenderer;
	_pTRenderer = new TextureRenderer(_pSetting->_nResultLen, _pSetting->_nResultLen);

	// 2.generate result
	double dbStep = _pSetting->_dbRadius * 2 / _pSetting->_nResultLen;
	for (size_t i = 0; i < _pSetting->_nResultLen; i++)
	{
		for (size_t j = 0; j < _pSetting->_nResultLen; j++)
		{
			// foreach grid points
			double x = -_pSetting->_dbRadius + dbStep*j;
			double y = -_pSetting->_dbRadius + dbStep*i;

			int nIndex = i*_pSetting->_nResultLen + j;


			double X[_nD] = { x,y };

			bool bMixedColor = true;
			if (bMixedColor) {
				double arrScores[3];
				_pClassifier->CalcScores(X, arrScores);
//				GLubyte bufData[4] = { (GLubyte)(arrScores[0] * 200)
//					, (GLubyte)(arrScores[1] * 200)
//					, (GLubyte)(arrScores[2] * 200)
//					, (GLubyte)255 };
				GLubyte bufData[4] = { (GLubyte)(arrScores[0] * 200)
					, (GLubyte)(0)
					, (GLubyte)(0)
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
}

void ClassificationLayer::UpdateLayer() {
	// train the parameter
	train();
	// reload the visualization of the classifier
	showClassifier();

//	generateHiddenLayerPoints();
}

void ClassificationLayer::generateHiddenLayerPoints() {
	for (size_t i = 0; i < g_nHiddenLayer; i++)
	{
		LabeledPoint pt(0, 0, 0);
		_pClassifier->GetHiddenLayer(i, pt._arrCoord);
		_vecHiddenlayerPt.push_back(pt);

	}
}


void ClassificationLayer::preparePointsForClassification() {
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
