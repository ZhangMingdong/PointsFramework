#pragma once
#include "NormalPointsLayer.h"
#include <QGLWidget>
#include "TextureRenderer.h"
#include "ConfidenceEllipse.h"
/*
	Layer to show the points obey single normal distribution
	Mingdong
	2017/06/21
*/
class SingleNormalPointsLayer :
	public NormalPointsLayer
{
public:
	SingleNormalPointsLayer(int number);
	virtual ~SingleNormalPointsLayer();
public:
	virtual void Draw();
	// initialization

	virtual void Initialize();
	// set the interpolation method
	virtual void SetMethod(int nMethod);

	// set the method to generate the source
	virtual void SetSource(int nSource);

	// update this layer, used for the display of classes in multiclass blue noise layer
	virtual void UpdateLayer();
protected:
	// generate the texture by confidence ellipse
	void generateTextureByConfidenceEllipse();

	// generate texture by rbf interpolation
	void generateTextureByRBF();

	// generate texture using simplicial depth and rbf
	void generateTextureBySimplicialDepth();

	// calculate simplicial depth
	void calculateSimplicialSepth(std::vector<DPoint3>& points);

	/*
		using Shepard's interpolation
		Scattered Data Interpolation for Computer Graphics, 2.1
	*/
	void generateTextureByShepards();

	/*
		generate texture by kernel density estimation
		4.1 from "Splatterplots: Overcoming Overdraw in Scatter Plots"
	*/
	void generateTextureByKDE();

	/*
		generate texture by kde using linear kernel
		5 from "Interactive Visualization of Streaming Data with Kernel Density Estimation"
	*/
	void generateTextureByKDE_LinearKernel();

	// generate source points
	void generatePoints();

	// do interpolation
	void doInterpolation();

	// generate dataset 1;
	void generateDataset1();

	// generate dataset 2;
	void generateDataset2();

	// generate dataset 3;
	void generateDataset3();

	// generate dataset 4;
	void generateDataset4();

	// generate dataset 5;
	// data as described in "Visualizing Confidence in Cluster-based Ensemble Weather Forecast Analyses"
	void generateDataset5();


	// generate dataset 6: MVN;
	void generateDataset6();

	// generate dataset 7: variance;
	void generateDataset7();	
	
	// generate dataset 8: mean;
	void generateDataset8();

	// generate dataset 9: 3 Gaussian with different density;
	void generateDataset9();

	void generateDataset10();

	// perform clustering
	void clustering();
protected:
	// the texture renderer
	TextureRenderer* _pTRenderer=NULL;
	// the confidence ellipse
	ConfidenceEllipse* _pCEllipse=NULL;

	/*
		interpolation method:
			0:confidence ellipse
			1:rbf
			2:sd+rdf
			3:kde
			4:linear kernel KDE
	*/
	int _nMethod = 0;

	/*
		source points
			0:normal
			1:dataset1
			2:dataset2
			3:dataset3
			4:dataset4
	*/
	int _nSource = 0;

	// length of source points
	int _nSourceLen = 0;

protected:		// for clustering

	// number of clusters
	int _nCluster=4;
	// four cluster
	std::vector<std::vector<DPoint3> > _clusteredPoints;
	
};

