#pragma once
#include "ILayer.h"
#include <vector>
// iris data
struct RawData
{
	double _arrBuf[17];	// for water data, 17 attribute
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
	void readData();		// read the data point
	void readWaterData();	// read water data points
	// run the dimensionality reduction algorithm 
	void runDR();
	// use the first two dimension to reduction, just as a contrast
	void usingFirst2D();
	// using pca to do reduction
	void usingPCA();
	// using mds to do reduction
	void usingMDS();
	// regularize the raw data to [0,1], (x-median)/median
	void regularize1();
	// (x-mean)/vairance
	void regularize2();
protected:
	// points
	std::vector<LabeledPoint> _vecPoints;
	// raw data
	std::vector<RawData> _vecRaw;
	int _nAttributes = 17;						// number of attributes
};

