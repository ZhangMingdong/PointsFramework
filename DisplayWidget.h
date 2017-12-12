#pragma once

#include <QWidget>

class QDoubleSpinBox;

class DisplayWidget : public QWidget
{
	Q_OBJECT

public:
	DisplayWidget(QWidget *parent=0);
	~DisplayWidget();
private:

	// spinbox of the eps
	QDoubleSpinBox *_pSpinBoxPointSize;

private:
	void createWidgets();
	void createLayout();
	void createConnections();

private slots:
	void updatePointSize(double ps);
signals:
	void pointSizeChanged(double ps);
};
