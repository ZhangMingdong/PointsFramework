#include "DisplayWidget.h"

#include <QDoubleSpinBox>
#include <QFormLayout>

DisplayWidget::DisplayWidget(QWidget *parent)
	: QWidget(parent)
{
	createWidgets();
	createLayout();
	createConnections();
}

DisplayWidget::~DisplayWidget()
{
}

void DisplayWidget::createWidgets() {
	_pSpinBoxPointSize = new QDoubleSpinBox;
	_pSpinBoxPointSize->setRange(1.0, 5.0);
	_pSpinBoxPointSize->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	_pSpinBoxPointSize->setValue(1.0);
	_pSpinBoxPointSize->setSingleStep(1.0);
	_pSpinBoxPointSize->setDecimals(1);
}

void DisplayWidget::createLayout() {
	QFormLayout *layout = new QFormLayout;
	layout->addRow(tr("DPoint3 Size:"), _pSpinBoxPointSize);
	setLayout(layout);
}

void DisplayWidget::createConnections() {
	connect(_pSpinBoxPointSize, SIGNAL(valueChanged(double)), this, SLOT(updatePointSize(double)));
}

void DisplayWidget::updatePointSize(double ps) {
	emit pointSizeChanged(ps);
}