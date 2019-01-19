#include "ControlWidget.h"

#include <QFormLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>

#include <QDebug>

ControlWidget::ControlWidget(QWidget *parent)
	: QWidget(parent)
{
	createWidgets();
	createLayout();
	createConnections();
}

ControlWidget::~ControlWidget()
{
}

void ControlWidget::createWidgets() {
	_pCheckBoxClustering = new QCheckBox;
	_pCheckBoxClustering->setText("Clustering");


	_pCheckBoxInterpolation = new QCheckBox;
	_pCheckBoxInterpolation->setText("Interpolation");
//	_pCheckBoxInterpolation->setChecked(true);

	_pCheckBoxSD = new QCheckBox;
	_pCheckBoxSD->setText("Simplicial Depth");

	_pCombData = new QComboBox;
	_pCombData->addItem("Normal", 0);
	_pCombData->addItem("Iris", 1);
	_pCombData->addItem("Water", 2);

	_pSpinBoxH = new QSpinBox;
	_pSpinBoxH->setRange(1, 10);
	_pSpinBoxH->setValue(1);
	_pSpinBoxH->setSingleStep(1);

}
void ControlWidget::createLayout() {
	QFormLayout *layout = new QFormLayout;
	layout->addRow(_pCheckBoxClustering);
	layout->addRow(_pCheckBoxInterpolation);
	layout->addRow(_pCheckBoxSD);
	layout->addRow(_pCombData);
	layout->addRow(tr("H:"), _pSpinBoxH);


	setLayout(layout);
}
void ControlWidget::createConnections() {
	connect(_pCheckBoxClustering, SIGNAL(clicked(bool)), this, SLOT(onClustering(bool)));
	connect(_pCheckBoxInterpolation, SIGNAL(clicked(bool)), this, SLOT(onInterpolation(bool)));
	connect(_pCheckBoxSD, SIGNAL(clicked(bool)), this, SLOT(onSD(bool)));


	connect(_pCombData, SIGNAL(currentIndexChanged(int)), this, SLOT(onUpdateData(int)));


	connect(_pSpinBoxH, SIGNAL(valueChanged(int)), this, SLOT(onUpdateH(int)));
}
void ControlWidget::onUpdateH(int h) {
	emit hChanged(h);
}

void ControlWidget::onUpdateData(int nData)
{
	emit dataChanged(nData);
}

void ControlWidget::onClustering(bool bClustering) {
	emit clusteringChanged(bClustering);
}
void ControlWidget::onInterpolation(bool bInterpolation) {
	emit interpolationChanged(bInterpolation);
}
void ControlWidget::onSD(bool bSD) {
	emit sdChanged(bSD);
}
