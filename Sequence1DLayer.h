#pragma once
#include "ILayer.h"
#include "MathTypes.h"
#include <vector>
/*
	Layer to show the 1D sequence
	Mingdong
	2017/06/21
*/
class Sequence1DLayer :
	public ILayer
{
public:
	Sequence1DLayer();
	virtual ~Sequence1DLayer();
public:
	virtual void Draw();
private:
	// sequence
	std::vector<Point> _sequence;
	// result of sequence
	std::vector<Point> _sequenceResult;
};

