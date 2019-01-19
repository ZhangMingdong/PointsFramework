#pragma once

// class used to record the setting for the layers
class LayerSetting
{
public:
	bool _bShowBg=false;		// whether show background

						// for sampling
	int _nSampleLen=10;
	int _nSamplePeriod=1;


	// for clustering
	int _nClusteringMethod = 0;
	int _nMinPts = 10;
	double _dbEps = 1.0;

	// if clustering
	bool _bClustering = false;

	// if interpolation
	bool _bInterpolation = false;

	// if calculate simplicial depth
	bool _bSD = false;

	double _dbPointSize = 1.0;

	// radius of the result
	double _dbRadius = 2;

	// length of the result
	int _nResultLen = 100;


	// length of the result
//	int _nResultLen = 800;
	// radius of the result
//	double _dbRadius = 4;


	// radius for phi function
	double _dbPhiRadius = .1;

	//== for 1D sequence==
	bool _b1DRBF = true;
	bool _b1DLagrangian = true;
	bool _b1DKDE = true;
	bool _b1DShepard = true;
	//== ~for 1D sequence==


	int _nH = 1;		// h parameter for kde

	enum MyEnum_Data
	{
		data_normal
		, data_iris
		, data_water
	};

	MyEnum_Data _enumData = data_normal;
};

