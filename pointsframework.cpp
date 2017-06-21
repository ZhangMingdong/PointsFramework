#include "pointsframework.h"

#include <QDockWidget>

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

	pControlWidget = new PointsControlWidget();


	QDockWidget *controlDockWidget = new QDockWidget(
		tr("Control"), this);
	controlDockWidget->setFeatures(features);
	controlDockWidget->setWidget(pControlWidget);
	addDockWidget(Qt::RightDockWidgetArea, controlDockWidget);

}

void PointsFramework::createConnections(){
	connect(pControlWidget->ui.pushButtonCalc, SIGNAL(pressed()), this, SLOT(onCalculateClicked()));
	connect(pControlWidget->ui.pushButtonCalcDC, SIGNAL(pressed()), this, SLOT(onCalculateDCClicked()));
	connect(pControlWidget->ui.pushButtonGenerateRandom, SIGNAL(pressed()), this, SLOT(onGenerateRandomClicked()));
	connect(pControlWidget->ui.pushButtonGenerateNormal, SIGNAL(pressed()), this, SLOT(onGenerateNormalClicked()));
	connect(pControlWidget->ui.pushButtonGenerateMultiNormal, SIGNAL(pressed()), this, SLOT(onGenerateMultiNormalClicked()));
	connect(pControlWidget->ui.pushButtonGenerateSequence, SIGNAL(pressed()), this, SLOT(onGenerateSequenceClicked()));
	connect(pControlWidget->ui.pushButtonGenerateSequence2D, SIGNAL(pressed()), this, SLOT(onGenerateSequence2DClicked()));
	connect(pControlWidget->ui.checkBoxPicking, SIGNAL(clicked(bool)), this, SLOT(onPickingClicked(bool)));


	connect(pControlWidget->ui.checkBoxShowBackground, SIGNAL(clicked(bool)), pWidget, SLOT(onShowBackground(bool)));
}

void PointsFramework::onGenerateRandomClicked(){
	// generate random points
 	pWidget->GenerateRandomPoints(pControlWidget->ui.spinBoxNum->value());

	pWidget->updateGL();

}

void PointsFramework::onGenerateNormalClicked() {
	// generate normal points
	pWidget->GenerateNormalPoints(pControlWidget->ui.spinBoxNum->value(), 0, 0, .4, .2);
	pWidget->updateGL();
}

void PointsFramework::onGenerateMultiNormalClicked() {
	// generate normal points
	pWidget->GenerateMVNPoints(pControlWidget->ui.spinBoxNum->value(), 0, 0, .4, .2);
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
	pControlWidget->ui.textEditResult->setPlainText(pControlWidget->ui.textEditResult->toPlainText() + s1 + s2 + s3 + s4);


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
	pControlWidget->ui.textEditResult->setPlainText(pControlWidget->ui.textEditResult->toPlainText()+s1+s2+s3+s4);

}

void PointsFramework::onPickingClicked(bool bChecked){
	pWidget->SetHandPoint();

}

