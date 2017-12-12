#ifndef POINTSFRAMEWORK_H
#define POINTSFRAMEWORK_H

#include <QtWidgets/QMainWindow>

#include "myglwidget.h"
#include "pointscontrolwidget.h"

class InterpolationWidget;
class ClusteringWidget;
class DisplayWidget;
class ControlWidget;

class PointsFramework : public QMainWindow
{
	Q_OBJECT

public:
	PointsFramework(QWidget *parent = 0);
	~PointsFramework();

private:
	MyGLWidget* pWidget;
	PointsControlWidget* _pControlWidget;
	InterpolationWidget* _pWidgetInterpolation;
	ClusteringWidget* _pWidgetClustering;
	DisplayWidget* _pWidgetDisplay;
	ControlWidget* _pWidgetControl;
protected:
	void createDockWidgets();
	void createConnections();
public slots:
	void onGenerateNormalClicked();
	void onGenerateBlueNoiseClicked();
	void onGenerateBlueNoiseNormalClicked();
	void onGenerateMulticlassBlueNoiseClicked();
	void onGenerateRandomClicked();
	void onGenerateMultiNormalClicked();
	void onGenerateSequenceClicked();
	void onGenerateSequence2DClicked();
	void onGenerateDRClicked();
	void onGenerateDataLayerClicked();
	void onCalculateClicked();
	void onCalculateDCClicked();
	void onPickingClicked(bool bChecked);
	void onGenerateSpiralClicked();
};

#endif // POINTSFRAMEWORK_H
