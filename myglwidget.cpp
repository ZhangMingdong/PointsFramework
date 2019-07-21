#include "myglwidget.h"

#include<QDebug>

MyGLWidget::MyGLWidget(QWidget *parent)
	: MyGLWidgetBase(parent), _pLayer(0)

{


	_pLayerSetting = new LayerSetting();

}

MyGLWidget::~MyGLWidget()
{
	delete _pLayerSetting;
	if (_pLayer)
	{
		delete _pLayer;
	}

}


void MyGLWidget::GenerateLayer(ILayer::EnumLayerType type, int number, int nClass) {
	if (_pLayer) delete _pLayer;
	_pLayer = ILayer::CreateLayer(type, _pLayerSetting,number,nClass);

}

void MyGLWidget::onShowBackground(bool bChecked) {
	_pLayerSetting->_bShowBg = bChecked;
	//if (_pLayer)
	//	_pLayer->ShowBackground(bChecked);
	updateGL();
}

void MyGLWidget::onUpdateLayer() {
	if (_pLayer)
		_pLayer->UpdateLayer();
	updateGL();
}

void MyGLWidget::onSetSampleLen(int nLen) {
	_pLayerSetting->_nSampleLen = nLen;
	if (_pLayer)
		_pLayer->Reset(_pLayerSetting->_nSampleLen, _pLayerSetting->_nSamplePeriod);
	updateGL();
}

void MyGLWidget::onSetSamplePeriod(int nPeriod) {
	_pLayerSetting->_nSamplePeriod = nPeriod;
	if (_pLayer) {
		_pLayer->Reset(_pLayerSetting->_nSampleLen, _pLayerSetting->_nSamplePeriod);
		_pLayer->UpdateLayer();

	}

	updateGL();
}

void MyGLWidget::updateMethod(int method) {
	if (_pLayer)
		_pLayer->SetMethod(method);
	updateGL();
}

void MyGLWidget::updateSource(int source) {
	if (_pLayer)
		_pLayer->SetSource(source);
	updateGL();
}

void MyGLWidget::updateRadius(double r) {
	_pLayerSetting->_dbRadius = r;
	if (_pLayer)
		_pLayer->UpdateLayer();
	updateGL();
}


void MyGLWidget::updateMinPts(int minPts) {
	_pLayerSetting->_nMinPts = minPts;
	if (_pLayer)
		_pLayer->UpdateLayer();
	updateGL();
}

void MyGLWidget::updateEps(double eps) {
	_pLayerSetting->_dbEps = eps;
	if (_pLayer)
		_pLayer->UpdateLayer();
	updateGL();
}

void MyGLWidget::updateClusteringMethod(int method) {
	_pLayerSetting->_nClusteringMethod = method;
	if (_pLayer)
		_pLayer->UpdateLayer();
	updateGL();
}


void MyGLWidget::updatePointSize(double ps) {
	_pLayerSetting->_dbPointSize = ps;

	updateGL();
}


void MyGLWidget::onClustering(bool b) {
	_pLayerSetting->_bClustering = b;
	qDebug() << "onClustering";
}
void MyGLWidget::onInterpolation(bool b) {
	_pLayerSetting->_bInterpolation = b;
	qDebug() << "onInterpolation";
}
void MyGLWidget::onSD(bool b) {
	_pLayerSetting->_bSD = b;
	qDebug() << "onInterpolation";
}


void MyGLWidget::onRBF(bool bState) {
	_pLayerSetting->_b1DRBF = bState;
	updateGL();
}
void MyGLWidget::onLagrangian(bool bState) {
	_pLayerSetting->_b1DLagrangian = bState;
	updateGL();
}
void MyGLWidget::onKDE(bool bState) {
	_pLayerSetting->_b1DKDE = bState;
	updateGL();
}
void MyGLWidget::onShepard(bool bState) {
	_pLayerSetting->_b1DShepard = bState;
	updateGL();

}

void MyGLWidget::onUpdateData(int nData) {
	qDebug() << "update Data" << nData;
	_pLayerSetting->_enumData = (LayerSetting::MyEnum_Data)nData;
}

void MyGLWidget::onUpdateH(int nH) {
	qDebug() << "update H" << nH;
	_pLayerSetting->_nH = nH;
	_pLayer->UpdateLayer();
	updateGL();
}

void MyGLWidget::draw() {
	if (_pLayer)
		_pLayer->Draw();
}


void MyGLWidget::onMouse(DPoint3& pt) {
	if (_pLayer)
		_pLayer->OnMouse(pt);
}