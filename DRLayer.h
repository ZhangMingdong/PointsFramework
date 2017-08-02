#pragma once
#include "ILayer.h"
#include <vector>
// iris data
struct RawData
{
	double _arrBuf[4];
	int _nLabel;
};
/*
	layer for dimensionality reduction
	mingdong
	2017/07/27
*/
class DRLayer :
	public ILayer
{
public:
	DRLayer();
	virtual ~DRLayer();
public:
	virtual void Draw();
protected:
	// read the data point
	void readData();
	// run the dimensionality reduction algorithm 
	void runDR();
	// use the first two dimension to reduction, just as a contrast
	void usingFirst2D();
	// using pca to do reduction
	void usingPCA();
	// regularize the raw data to [0,1], (x-median)/median
	void regularize1();
	// (x-mean)/vairance
	void regularize2();
protected:
	// points
	std::vector<LabeledPoint> _vecPoints;
	// raw data
	std::vector<RawData> _vecRaw;
};

