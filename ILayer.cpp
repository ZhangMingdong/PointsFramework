#include "ILayer.h"

#include "Sequence1DLayer.h"
#include "Sequence2DLayer.h"



ILayer::ILayer():_bShowBackground(false)
{
}


ILayer::~ILayer()
{
}

ILayer* ILayer::CreateLayer(EnumLayerType type) {
	switch (type)
	{
	case ILayer::LT_Sequence_1D:
		return new Sequence1DLayer();
		break;
	case ILayer::LT_Sequence_2D:
		return new Sequence2DLayer();
		break;
	default:
		return 0;
		break;
	}
}

double funPhi(double r) {
	//	return exp(-r*r );
//	return exp(-r*r * 16);
//	return exp(-r*r * 4);
	return exp(-r*r / 4);
	//	return exp(-r*r / 16);
}