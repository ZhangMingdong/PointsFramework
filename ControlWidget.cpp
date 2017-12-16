#include "ControlWidget.h"

#include <QFormLayout>
#include <QCheckBox>

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


}
void ControlWidget::createLayout() {
	QFormLayout *layout = new QFormLayout;
	layout->addRow(_pCheckBoxClustering);
	layout->addRow(_pCheckBoxInterpolation);
	layout->addRow(_pCheckBoxSD);
	setLayout(layout);
}
void ControlWidget::createConnections() {
	connect(_pCheckBoxClustering, SIGNAL(clicked(bool)), this, SLOT(onClustering(bool)));
	connect(_pCheckBoxInterpolation, SIGNAL(clicked(bool)), this, SLOT(onInterpolation(bool)));
	connect(_pCheckBoxSD, SIGNAL(clicked(bool)), this, SLOT(onSD(bool)));
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
