#pragma once

#include <QWidget>
class QCheckBox;

/*
	widget to set the parameter of the interpolation
*/
class Sequence1DWidget : public QWidget
{
	Q_OBJECT

public:
	Sequence1DWidget(QWidget *parent = 0);
	~Sequence1DWidget();
private:
	QCheckBox* _pCheckBoxRBF;
	QCheckBox* _pCheckBoxLagrangian;
	QCheckBox* _pCheckBoxKDE;
	QCheckBox* _pCheckBoxShepard;
private:
	void createWidgets();
	void createLayout();
	void createConnections();

private slots:
	void onRBF(bool bState);
	void onLagrangian(bool bState);
	void onKDE(bool bState);
	void onShepard(bool bState);
signals:
	void RBFChanged(bool bState);
	void LagrangianChanged(bool bState);
	void KDEChanged(bool bState);
	void ShepardChanged(bool bState);


};
