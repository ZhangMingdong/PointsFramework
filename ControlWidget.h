#pragma once

#include <QWidget>

class QCheckBox;
class QComboBox;
class QSpinBox;

class ControlWidget : public QWidget
{
	Q_OBJECT

public:
	ControlWidget(QWidget *parent=0);
	~ControlWidget();
private:	
	QCheckBox* _pCheckBoxClustering;		// whether clustering	
	QCheckBox* _pCheckBoxInterpolation;		// whether interpolate	
	QCheckBox* _pCheckBoxSD;				// whether calculate simplicial depth
	QComboBox *_pCombData;					// data source
	QSpinBox *_pSpinBoxH;					// h parameter of the KDE

private:
	void createWidgets();
	void createLayout();
	void createConnections();

private slots:
	void onClustering(bool bClustering);
	void onInterpolation(bool bInterpolation);
	void onSD(bool bSD);
	void onUpdateData(int nData);
	void onUpdateH(int h);
signals:
	void clusteringChanged(bool bClustering);
	void interpolationChanged(bool bInterpolation);
	void sdChanged(bool bSD);
	void dataChanged(int nData);
	void hChanged(int nH);
};
