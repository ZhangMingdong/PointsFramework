#include "pointsframework.h"
#include "InterpolationWidget.h"
#include "ClusteringWidget.h"
#include <QDockWidget>
#include "DisplayWidget.h"
#include "ControlWidget.h"

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
	connect(_pControlWidget->ui.pushButtonGenerateMulticlassBlueNoise, SIGNAL(pressed()), this, SLOT(onGenerateMulticlassBlueNoiseClicked()));
	connect(_pControlWidget->ui.pushButtonGenerateNormal, SIGNAL(pressed()), this, SLOT(onGenerateNormalClicked()));
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
}

void PointsFramework::onGenerateRandomClicked(){
	// generate random points
 	pWidget->GenerateRandomPoints(_pControlWidget->ui.spinBoxNum->value());

	pWidget->updateGL();

}

void PointsFramework::onGenerateBlueNoiseClicked() {
	// generate normal points

	long t1 = GetTickCount();
	pWidget->GenerateBlueNoise(_pControlWidget->ui.spinBoxNum->value());
	int t = GetTickCount() - t1; 
	QString s1 = QStringLiteral("生成蓝噪声。\n计算时间：") + QString::number(t) + QStringLiteral("dms。\n");
	_pControlWidget->ui.textEditResult->setPlainText(_pControlWidget->ui.textEditResult->toPlainText() + s1);

	pWidget->updateGL();
}
void PointsFramework::onGenerateSpiralClicked() {
	// generate normal points

	long t1 = GetTickCount();
	pWidget->GenerateSpiralPoints();
	int t = GetTickCount() - t1;
	QString s1 = QStringLiteral("生成蓝噪声。\n计算时间：") + QString::number(t) + QStringLiteral("dms。\n");
	_pControlWidget->ui.textEditResult->setPlainText(_pControlWidget->ui.textEditResult->toPlainText() + s1);

	pWidget->updateGL();
}
void PointsFramework::onGenerateBlueNoiseNormalClicked() {
	// generate normal points

	long t1 = GetTickCount();
	pWidget->GenerateBlueNoiseNormal(_pControlWidget->ui.spinBoxNum->value());
	int t = GetTickCount() - t1;
	QString s1 = QStringLiteral("生成正态蓝噪声。\n计算时间：") + QString::number(t) + QStringLiteral("dms。\n");
	_pControlWidget->ui.textEditResult->setPlainText(_pControlWidget->ui.textEditResult->toPlainText() + s1);

	pWidget->updateGL();
}
void PointsFramework::onGenerateMulticlassBlueNoiseClicked() {
	// generate normal points

	long t1 = GetTickCount();
	pWidget->GenerateMulticlassBlueNoise(_pControlWidget->ui.spinBoxNum->value());
	int t = GetTickCount() - t1; 
	QString s1 = QStringLiteral("生成多组蓝噪声。\n计算时间：") + QString::number(t) + QStringLiteral("dms。\n");
	_pControlWidget->ui.textEditResult->setPlainText(_pControlWidget->ui.textEditResult->toPlainText() + s1);

	pWidget->updateGL();
}
void PointsFramework::onGenerateNormalClicked() {
	// generate normal points
	pWidget->GenerateNormalPoints(_pControlWidget->ui.spinBoxNum->value());
	pWidget->updateGL();
}

void PointsFramework::onGenerateMultiNormalClicked() {
	// generate normal points
	pWidget->GenerateMVNPoints(_pControlWidget->ui.spinBoxNum->value());
	pWidget->updateGL();

}

void PointsFramework::onGenerateSequenceClicked() {
	// generate sequence
	pWidget->GenerateSequence();
	pWidget->updateGL();

}

void PointsFramework::onGenerateSequence2DClicked() {
	// generate sequence
	pWidget->GenerateSequence2D();
	pWidget->updateGL();

}
void PointsFramework::onGenerateDRClicked() {
	// generate sequence
	pWidget->GenerateDR();
	pWidget->updateGL();

}void PointsFramework::onGenerateDataLayerClicked() {
	// generate sequence
	pWidget->GenerateDataLayer();
	pWidget->updateGL();

}
void PointsFramework::onCalculateClicked(){
	long t1 = GetTickCount();
	DistanceAndIndices result = pWidget->CalcNearestPairOfPoints();
	pWidget->updateGL();
	int t = GetTickCount() - t1;
	QString s1 = QStringLiteral("找到最近点对。\n计算时间：") + QString::number(t) + QStringLiteral("dms。\n");
	QString s2 = QStringLiteral("最近点对的索引：") + QString::number(result._nIndex1) + ","
		+ QString::number(result._nIndex2) + QStringLiteral("。\n");
	QString s3 = QStringLiteral("距离：") + QString::number(result._dbDis) + QStringLiteral("。\n");
	QString s4 = "==============\n";
	_pControlWidget->ui.textEditResult->setPlainText(_pControlWidget->ui.textEditResult->toPlainText() + s1 + s2 + s3 + s4);


}

void PointsFramework::onCalculateDCClicked(){
	long t1 = GetTickCount();
	DistanceAndIndices result = pWidget->CalcNearestPairOfPoints_DC();
	pWidget->updateGL();
	int t = GetTickCount() - t1;
	QString s1 = QStringLiteral("分治找到最近点对。\n计算时间：") + QString::number(t) + QStringLiteral("dms。\n");
	QString s2 = QStringLiteral("最近点对的索引：") + QString::number(result._nIndex1) + ","
		+ QString::number(result._nIndex2) + QStringLiteral("。\n");
	QString s3= QStringLiteral("距离：") + QString::number(result._dbDis) + QStringLiteral("。\n");
	QString s4 = "==============\n";
	_pControlWidget->ui.textEditResult->setPlainText(_pControlWidget->ui.textEditResult->toPlainText()+s1+s2+s3+s4);

}

void PointsFramework::onPickingClicked(bool bChecked){
	pWidget->SetHandPoint();

}

