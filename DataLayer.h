#pragma once
#include "ILayer.h"
#include "TextureRenderer.h"

/*
	layer used to show data
*/
class DataLayer :
	public ILayer
{
public:
	DataLayer();
	virtual ~DataLayer();
public:
	virtual void Draw();
protected:
	// points
	std::vector<LabeledPoint> _vecPoints;
	// to render the texture
	TextureRenderer* _pTRenderer;
};

