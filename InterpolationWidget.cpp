#include "InterpolationWidget.h"

#include <QComboBox>
#include <QFormLayout>

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


	_pCombSource = new QComboBox;
	_pCombSource->addItem("Normal", 0);
	_pCombSource->addItem("Dataset1", 1);
	_pCombSource->addItem("Dataset2", 2);
	_pCombSource->addItem("Dataset3", 3);
	_pCombSource->addItem("Dataset4", 4);
}

void InterpolationWidget::createLayout() {

	QFormLayout *layout = new QFormLayout;
	layout->addRow(tr("Method"), _pCombMethod);
	layout->addRow(tr("Source"), _pCombSource);
	setLayout(layout);
}

void InterpolationWidget::createConnections() {

	connect(_pCombMethod, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMethod(int)));
	connect(_pCombSource, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSource(int)));
}

void InterpolationWidget::updateMethod(int method)
{
	emit methodChanged(method);
}
void InterpolationWidget::updateSource(int source)
{
	emit sourceChanged(source);
}


