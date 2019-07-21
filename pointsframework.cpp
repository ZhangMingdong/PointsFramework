#include "pointsframework.h"
#include "InterpolationWidget.h"
#include "ClusteringWidget.h"
#include <QDockWidget>
#include "DisplayWidget.h"
#include "ControlWidget.h"
#include "Sequence1DWidget.h"

#include<QDebug>

PointsFramework::PointsFramework(QWidget *parent)
	: QMainWindow(parent)
{

	pWidget = new MyGLWidget(this);
	this->setCentralWidget(pWidget);


	createDockWidgets();
	createConnections();
	this->showMaximized();
}

PointsFramework::~PointsFramework()
{

}

void PointsFramework::createDockWidgets(){
	setDockOptions(QMainWindow::AnimatedDocks);
	QDockWidget::DockWidgetFeatures features =
		QDockWidget::DockWidgetMovable |
		QDockWidget::DockWidgetFloatable;

	_pControlWidget = new PointsControlWidget();


	QDockWidget *controlDockWidget = new QDockWidget(
		tr("Control"), this);
	controlDockWidget->setFeatures(features);
	controlDockWidget->setWidget(_pControlWidget);
	addDockWidget(Qt::RightDockWidgetArea, controlDockWidget);


	_pWidgetInterpolation = new InterpolationWidget();
	QDockWidget *pDockWidgetInterpolation = new QDockWidget(tr("Interpolation"), this);
	pDockWidgetInterpolation->setFeatures(features);
	pDockWidgetInterpolation->setWidget(_pWidgetInterpolation);
	addDockWidget(Qt::LeftDockWidgetArea, pDockWidgetInterpolation);

	_pWidgetSequence1D = new Sequence1DWidget();
	QDockWidget *pDockWidgetSequence1D = new QDockWidget(tr("Sequence1D"), this);
	pDockWidgetSequence1D->setFeatures(features);
	pDockWidgetSequence1D->setWidget(_pWidgetSequence1D);
	addDockWidget(Qt::LeftDockWidgetArea, pDockWidgetSequence1D);

	_pWidgetClustering = new ClusteringWidget();
	QDockWidget *pDockWidgetClustering = new QDockWidget(tr("Clustering"), this);
	pDockWidgetClustering->setFeatures(features);
	pDockWidgetClustering->setWidget(_pWidgetClustering);
	addDockWidget(Qt::LeftDockWidgetArea, pDockWidgetClustering);

	_pWidgetDisplay = new DisplayWidget();
	QDockWidget *pDockWidgetDisplay = new QDockWidget(tr("Display"), this);
	pDockWidgetDisplay->setFeatures(features);
	pDockWidgetDisplay->setWidget(_pWidgetDisplay);
	addDockWidget(Qt::LeftDockWidgetArea, pDockWidgetDisplay);

	_pWidgetControl = new ControlWidget();
	QDockWidget *pDockWidgetControl = new QDockWidget(tr("Control"), this);
	pDockWidgetControl->setFeatures(features);
	pDockWidgetControl->setWidget(_pWidgetControl);
	addDockWidget(Qt::LeftDockWidgetArea, pDockWidgetControl);

}

void PointsFramework::createConnections(){
	connect(_pControlWidget->ui.pushButtonCalc, SIGNAL(pressed()), this, SLOT(onCalculateClicked()));
	connect(_pControlWidget->ui.pushButtonCalcDC, SIGNAL(pressed()), this, SLOT(onCalculateDCClicked()));
	connect(_pControlWidget->ui.pushButtonGenerateRandom, SIGNAL(pressed()), this, SLOT(onGenerateRandomClicked()));
	connect(_pControlWidget->ui.pushButtonGenerateBlueNoise, SIGNAL(pressed()), this, SLOT(onGenerateBlueNoiseClicked()));
	connect(_pControlWidget->ui.pushButtonGenerateSpiral, SIGNAL(pressed()), this, SLOT(onGenerateSpiralClicked()));
	connect(_pControlWidget->ui.pushButtonGenerateBlueNoiseNormal, SIGNAL(pressed()), this, SLOT(onGenerateBlueNoiseNormalClicked()));
	connect(_pControlWidget->ui.pushButtonGenerateTimeSeries, SIGNAL(pressed()), this, SLOT(onGenerateTimeSeriesClicked()));
	connect(_pControlWidget->ui.pushButtonGenerateNormal, SIGNAL(pressed()), this, SLOT(onGenerateNormalClicked()));
	connect(_pControlWidget->ui.pushButtonGenerateGrid, SIGNAL(pressed()), this, SLOT(onGenerateGridClicked()));
	connect(_pControlWidget->ui.pushButtonGenerateMultiNormal, SIGNAL(pressed()), this, SLOT(onGenerateMultiNormalClicked()));
	connect(_pControlWidget->ui.pushButtonGenerateSequence, SIGNAL(pressed()), this, SLOT(onGenerateSequenceClicked()));
	connect(_pControlWidget->ui.pushButtonGenerateSequence2D, SIGNAL(pressed()), this, SLOT(onGenerateSequence2DClicked()));
	connect(_pControlWidget->ui.pushButtonGenerateDR, SIGNAL(pressed()), this, SLOT(onGenerateDRClicked()));
	connect(_pControlWidget->ui.pushButtonShowData, SIGNAL(pressed()), this, SLOT(onGenerateDataLayerClicked()));
	connect(_pControlWidget->ui.checkBoxPicking, SIGNAL(clicked(bool)), this, SLOT(onPickingClicked(bool)));


	connect(_pControlWidget->ui.checkBoxShowBackground, SIGNAL(clicked(bool)), pWidget, SLOT(onShowBackground(bool)));
	connect(_pControlWidget->ui.pushButtonUpdateLayer, SIGNAL(pressed()), pWidget, SLOT(onUpdateLayer()));

	connect(_pControlWidget->ui.horizontalSliderLen, SIGNAL(valueChanged(int)), pWidget, SLOT(onSetSampleLen(int)));
	connect(_pControlWidget->ui.horizontalSliderPeriod, SIGNAL(valueChanged(int)), pWidget, SLOT(onSetSamplePeriod(int)));

	// interpolation widget
	connect(_pWidgetInterpolation, SIGNAL(methodChanged(int)), pWidget, SLOT(updateMethod(int)));
	connect(_pWidgetInterpolation, SIGNAL(sourceChanged(int)), pWidget, SLOT(updateSource(int)));
	connect(_pWidgetInterpolation, SIGNAL(radiusChanged(double)), pWidget, SLOT(updateRadius(double)));

	// clustering widget
	connect(_pWidgetClustering, SIGNAL(methodChanged(int)), pWidget, SLOT(updateClusteringMethod(int)));
	connect(_pWidgetClustering, SIGNAL(minPtsChanged(int)), pWidget, SLOT(updateMinPts(int)));
	connect(_pWidgetClustering, SIGNAL(epsChanged(double)), pWidget, SLOT(updateEps(double)));

	// display widget
	connect(_pWidgetDisplay, SIGNAL(pointSizeChanged(double)), pWidget, SLOT(updatePointSize(double)));

	// control
	connect(_pWidgetControl, SIGNAL(clusteringChanged(bool)), pWidget, SLOT(onClustering(bool)));
	connect(_pWidgetControl, SIGNAL(interpolationChanged(bool)), pWidget, SLOT(onInterpolation(bool)));
	connect(_pWidgetControl, SIGNAL(sdChanged(bool)), pWidget, SLOT(onSD(bool)));

	connect(_pWidgetControl, SIGNAL(dataChanged(int)), pWidget, SLOT(onUpdateData(int)));
	connect(_pWidgetControl, SIGNAL(hChanged(int)), pWidget, SLOT(onUpdateH(int)));

	// sequence 1D
	connect(_pWidgetSequence1D, SIGNAL(RBFChanged(bool)), pWidget, SLOT(onRBF(bool)));
	connect(_pWidgetSequence1D, SIGNAL(LagrangianChanged(bool)), pWidget, SLOT(onLagrangian(bool)));
	connect(_pWidgetSequence1D, SIGNAL(KDEChanged(bool)), pWidget, SLOT(onKDE(bool)));
	connect(_pWidgetSequence1D, SIGNAL(ShepardChanged(bool)), pWidget, SLOT(onShepard(bool)));


}

void PointsFramework::onGenerateRandomClicked(){
	// generate random points
	pWidget->GenerateLayer(ILayer::LT_Random,_pControlWidget->ui.spinBoxNum->value());

	pWidget->updateGL();

}

void PointsFramework::onGenerateBlueNoiseClicked() {
	// generate normal points

	long t1 = GetTickCount();
	pWidget->GenerateLayer(ILayer::LT_Random_Blue,_pControlWidget->ui.spinBoxNum->value(), _pControlWidget->ui.spinBoxClass->value());
	int t = GetTickCount() - t1; 
	QString s1 = QStringLiteral("������������\n����ʱ�䣺") + QString::number(t) + QStringLiteral("dms��\n");
	_pControlWidget->ui.textEditResult->setPlainText(_pControlWidget->ui.textEditResult->toPlainText() + s1);

	pWidget->updateGL();
}
void PointsFramework::onGenerateSpiralClicked() {
	// generate normal points
	long t1 = GetTickCount();
	pWidget->GenerateLayer(ILayer::LT_Spiral);
	int t = GetTickCount() - t1;
	QString s1 = QStringLiteral("������������\n����ʱ�䣺") + QString::number(t) + QStringLiteral("dms��\n");
	_pControlWidget->ui.textEditResult->setPlainText(_pControlWidget->ui.textEditResult->toPlainText() + s1);

	pWidget->updateGL();
}
void PointsFramework::onGenerateBlueNoiseNormalClicked() {
	// generate normal points

	long t1 = GetTickCount();
	pWidget->GenerateLayer(ILayer::LT_Random_Blue_Mult,_pControlWidget->ui.spinBoxNum->value());
	int t = GetTickCount() - t1;
	QString s1 = QStringLiteral("������̬��������\n����ʱ�䣺") + QString::number(t) + QStringLiteral("dms��\n");
	_pControlWidget->ui.textEditResult->setPlainText(_pControlWidget->ui.textEditResult->toPlainText() + s1);

	pWidget->updateGL();
}
void PointsFramework::onGenerateTimeSeriesClicked() {
	// generate normal points

	long t1 = GetTickCount();
	pWidget->GenerateLayer(ILayer::LT_Time_Series,_pControlWidget->ui.spinBoxNum->value(), _pControlWidget->ui.spinBoxClass->value());
	int t = GetTickCount() - t1; 
	QString s1 = QStringLiteral("����ʱ�����ݡ�\n����ʱ�䣺") + QString::number(t) + QStringLiteral("dms��\n");
	_pControlWidget->ui.textEditResult->setPlainText(_pControlWidget->ui.textEditResult->toPlainText() + s1);

	pWidget->updateGL();
}
void PointsFramework::onGenerateNormalClicked() {
	// generate normal points
	pWidget->GenerateLayer(ILayer::LT_Normal_Single,_pControlWidget->ui.spinBoxNum->value());
	pWidget->updateGL();
}
void PointsFramework::onGenerateGridClicked() {
	// generate grid points
	pWidget->GenerateLayer(ILayer::LT_Grid, 100);
	pWidget->updateGL();
}

void PointsFramework::onGenerateMultiNormalClicked() {
	// generate normal points
	pWidget->GenerateLayer(ILayer::LT_Normal_Multi,_pControlWidget->ui.spinBoxNum->value());
	pWidget->updateGL();

}

void PointsFramework::onGenerateSequenceClicked() {
	// generate sequence
	pWidget->GenerateLayer(ILayer::LT_Sequence_1D);
	pWidget->updateGL();

}

void PointsFramework::onGenerateSequence2DClicked() {
	// generate sequence
	pWidget->GenerateLayer(ILayer::LT_Sequence_2D);
	pWidget->updateGL();

}
void PointsFramework::onGenerateDRClicked() {
	// generate sequence
	pWidget->GenerateLayer(ILayer::LT_DR);
	pWidget->updateGL();

}void PointsFramework::onGenerateDataLayerClicked() {
	// generate sequence
	pWidget->GenerateLayer(ILayer::LT_Data);
	pWidget->updateGL();

}
void PointsFramework::onCalculateClicked(){


}

void PointsFramework::onCalculateDCClicked(){

}

void PointsFramework::onPickingClicked(bool bChecked){


}

