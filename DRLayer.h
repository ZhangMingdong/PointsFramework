#pragma once
#include "ClassificationLayer.h"
#include <vector>
class TextureRenderer;

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
class DRLayer : public ClassificationLayer
{
public:
	DRLayer();
	virtual ~DRLayer();
public:
	//virtual void Draw();
	//virtual void UpdateLayer();
protected:
	virtual void generatePoints();
protected:	
	void readIrisData();	// read the iris
	void readWaterData();	// read water data points	
	void runDR();			// run the dimensionality reduction algorithm 	
	void usingFirst2D();	// use the first two dimension to reduction, just as a contrast	
	void usingPCA();		// using pca to do reduction	
	void usingMDS();		// using mds to do reduction	
	void regularize1();		// regularize the raw data to [0,1], (x-median)/median
	void regularize2();		// (x-mean)/vairance
	void generateTextureByKDE();	// generate texture by KDE
protected:	
	// std::vector<LabeledPoint> _vecPoints;		// points	
	std::vector<RawData> _vecRaw;				// raw data
	int _nAttributes = 0;						// number of attributes
	TextureRenderer* _pTRenderer = NULL;		// the texture renderer
};

