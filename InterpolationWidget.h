#pragma once

#include <QWidget>
class QComboBox;

/*
	widget to set the parameter of the interpolation
*/
class InterpolationWidget : public QWidget
{
	Q_OBJECT

public:
	InterpolationWidget(QWidget *parent = 0);
	~InterpolationWidget();
private:
	// control the interpolation method
	QComboBox *_pCombMethod;

	// control the generation of the source points
	QComboBox *_pCombSource;

private:
	void createWidgets();
	void createLayout();
	void createConnections();

private slots:
	void updateMethod(int method);
	void updateSource(int method);
signals:
	void methodChanged(int method);
	void sourceChanged(int method);


};
