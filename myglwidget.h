#pragma once

#include"MyGLWidgetBase.h"

#include "ILayer.h"

class LayerSetting;

class MyGLWidget : public MyGLWidgetBase
{
	Q_OBJECT

public:
	MyGLWidget(QWidget *parent);
	~MyGLWidget();

public:// generate layer
	void GenerateLayer(ILayer::EnumLayerType type, int number=0, int nClass=1);
protected:

	ILayer *_pLayer = NULL;	// current layer

	// setting for the layers
	LayerSetting* _pLayerSetting = NULL;

public slots:
	void onShowBackground(bool bChecked);
	void onUpdateLayer();
	void onSetSampleLen(int nLen);
	void onSetSamplePeriod(int nPeriod);
	void updateMethod(int method);
	void updateSource(int source);
	void updateRadius(double r);
	// for clustering
	void updateMinPts(int minPts);
	void updateEps(double eps);
	void updateClusteringMethod(int method);
	// for display
	void updatePointSize(double ps);
	// for control
	void onClustering(bool b);
	void onInterpolation(bool b);
	void onSD(bool b);
	// for 1d sequence
	void onRBF(bool bState);
	void onLagrangian(bool bState);
	void onKDE(bool bState);
	void onShepard(bool bState);


	void onUpdateData(int);
	void onUpdateH(int);
protected:

	virtual void draw();
	virtual void onMouse(DPoint3& pt);
};

