#include "IMyClassifier.h"
#include "SoftMaxClassifier.h"
#include "AnnClassifier.h"
#include "L3AnnClassifier.h"

//#define L3_ANN

IMyClassifier::IMyClassifier() :_nPoints(0)
, _nD(0)
, _nClass(0)
{
}

void IMyClassifier::initialize(int nPoints, int nD, int nClass) {
	_nPoints = nPoints;
	_nD = nD;
	_nClass = nClass;
	initializeImp();
}

IMyClassifier::~IMyClassifier()
{
}

IMyClassifier* IMyClassifier::CreateClassifier(Enum_Classifier type, int nPoints, int nD, int nClass, int nHiddden) {
	IMyClassifier* pClassifier = 0;
	switch (type)
	{
	case IMyClassifier::SoftMax:
		pClassifier = new SoftMaxClassifier();
		break;
	case IMyClassifier::Ann:
#ifdef L3_ANN
		pClassifier = new L3AnnClassifier(nHiddden);
#else
		pClassifier = new AnnClassifier(nHiddden);
#endif
		break;
	default:
		break;
	}
	if(pClassifier)
		pClassifier->initialize(nPoints, nD, nClass);
	return pClassifier;
}