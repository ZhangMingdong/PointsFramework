#include "Sequence1DWidget.h"

#include <QCheckBox>
#include <QFormLayout>

Sequence1DWidget::Sequence1DWidget(QWidget *parent)
	: QWidget(parent)
{
	createWidgets();
	createLayout();
	createConnections();
}

Sequence1DWidget::~Sequence1DWidget(){

}

void Sequence1DWidget::createWidgets() {
	_pCheckBoxRBF = new QCheckBox;
	_pCheckBoxRBF->setText("RBF");
	_pCheckBoxLagrangian = new QCheckBox;
	_pCheckBoxLagrangian->setText("Lagrangian");
	_pCheckBoxKDE = new QCheckBox;
	_pCheckBoxKDE->setText("KDE");
	_pCheckBoxShepard = new QCheckBox;
	_pCheckBoxShepard->setText("Shepard");
}

void Sequence1DWidget::createLayout() {

	QFormLayout *layout = new QFormLayout;
	layout->addRow(_pCheckBoxRBF);
	layout->addRow(_pCheckBoxLagrangian);
	layout->addRow(_pCheckBoxKDE);
	layout->addRow(_pCheckBoxShepard);
	setLayout(layout);
}

void Sequence1DWidget::createConnections() {
	connect(_pCheckBoxRBF, SIGNAL(clicked(bool)), this, SLOT(onRBF(bool)));
	connect(_pCheckBoxLagrangian, SIGNAL(clicked(bool)), this, SLOT(onLagrangian(bool)));
	connect(_pCheckBoxKDE, SIGNAL(clicked(bool)), this, SLOT(onKDE(bool)));
	connect(_pCheckBoxShepard, SIGNAL(clicked(bool)), this, SLOT(onShepard(bool)));
}


void Sequence1DWidget::onRBF(bool bState) {
	emit RBFChanged(bState);
}
void Sequence1DWidget::onLagrangian(bool bState) {
	emit LagrangianChanged(bState);
}
void Sequence1DWidget::onKDE(bool bState) {
	emit KDEChanged(bState);
}
void Sequence1DWidget::onShepard(bool bState) {
	emit ShepardChanged(bState);
}