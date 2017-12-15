#include "InterpolationWidget.h"

#include <QComboBox>
#include <QFormLayout>
#include <QDoubleSpinBox>

InterpolationWidget::InterpolationWidget(QWidget *parent)
	: QWidget(parent)
{
	createWidgets();
	createLayout();
	createConnections();
}

InterpolationWidget::~InterpolationWidget(){

}

void InterpolationWidget::createWidgets() {
	_pCombMethod = new QComboBox;
	_pCombMethod->addItem("Confidence Ellipse", 0);
	_pCombMethod->addItem("Radial Basis Function", 1);
	_pCombMethod->addItem("RBF+Simplicial Depth", 2);
	_pCombMethod->addItem("Kernel Density Estimation", 3);
	_pCombMethod->addItem("Linear Kernel KDE", 4);
	_pCombMethod->addItem("Shepard's Interpolation", 5);


	_pCombSource = new QComboBox;
	_pCombSource->addItem("Normal", 0);
	_pCombSource->addItem("Dataset1", 1);
	_pCombSource->addItem("Dataset2", 2);
	_pCombSource->addItem("Dataset3", 3);
	_pCombSource->addItem("Dataset4", 4);
	_pCombSource->addItem("Dataset5", 5);
	_pCombSource->addItem("Dataset6(MVN)", 6);
	_pCombSource->addItem("Dataset7(Var)", 7);
	_pCombSource->addItem("Dataset8(Mean)", 8);
	_pCombSource->addItem("Dataset9(3 Gaussian with different density)", 9);
	_pCombSource->addItem("Dataset10", 10);

	_pSpinBoxR = new QDoubleSpinBox;
	_pSpinBoxR->setRange(0.1, 1.0);
	_pSpinBoxR->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	_pSpinBoxR->setValue(1.0);
	_pSpinBoxR->setSingleStep(0.1);
	_pSpinBoxR->setDecimals(1);
}

void InterpolationWidget::createLayout() {

	QFormLayout *layout = new QFormLayout;
	layout->addRow(tr("Method:"), _pCombMethod);
	layout->addRow(tr("Source:"), _pCombSource);
	layout->addRow(tr("Radius:"), _pSpinBoxR);
	setLayout(layout);
}

void InterpolationWidget::createConnections() {

	connect(_pCombMethod, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMethod(int)));
	connect(_pCombSource, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSource(int)));
	connect(_pSpinBoxR, SIGNAL(valueChanged(double)), this, SLOT(updateRadius(double)));
}

void InterpolationWidget::updateMethod(int method)
{
	emit methodChanged(method);
}

void InterpolationWidget::updateSource(int source)
{
	emit sourceChanged(source);
}

void InterpolationWidget::updateRadius(double r)
{
	emit radiusChanged(r);
}

