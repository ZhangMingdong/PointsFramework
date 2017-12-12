#pragma once

#include <QWidget>

class QCheckBox;

class ControlWidget : public QWidget
{
	Q_OBJECT

public:
	ControlWidget(QWidget *parent=0);
	~ControlWidget();
private:
	// whether clustering
	QCheckBox* _pCheckBoxClustering;
	// whether interpolate
	QCheckBox* _pCheckBoxInterpolation;
	// whether calculate simplicial depth
	QCheckBox* _pCheckBoxSD;

private:
	void createWidgets();
	void createLayout();
	void createConnections();

private slots:
	void onClustering(bool bClustering);
	void onInterpolation(bool bInterpolation);
	void onSD(bool bSD);
signals:
	void clusteringChanged(bool bClustering);
	void interpolationChanged(bool bInterpolation);
	void sdChanged(bool bSD);
};
