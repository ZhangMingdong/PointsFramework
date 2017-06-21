#include "pointscontrolwidget.h"

PointsControlWidget::PointsControlWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.spinBoxNum->setRange(0, 1000000);
	ui.spinBoxNum->setValue(1000);
}

PointsControlWidget::~PointsControlWidget()
{

}
