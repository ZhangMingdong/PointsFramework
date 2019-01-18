#pragma once
#include "ILayer.h"
#include <vector>
#include <QGLWidget>
#include "RBFInterpolator.h"

class TextureRenderer;
/*
	Layer to show the 2D sequence
	Mingdong
	2017/06/21
	*/
class Sequence2DLayer :
	public ILayer
{
public:
	Sequence2DLayer();
	virtual ~Sequence2DLayer();
public:
	virtual void Draw();
	// initialization
	virtual void Initialize();


	// set the interpolation method
	virtual void SetMethod(int nMethod);
private:
	// 2D sequence
	std::vector<DPoint3> _sequence;

	// the interpolator
	RBFInterpolator _interpolator;


	// the texture renderer
	TextureRenderer* _pTRenderer = NULL;

	/*
		interpolation method:
			0:confidence ellipse
			1:rbf
			2:sd+rdf
			3:kde
	*/
	int _nMethod = 0;
protected:
	// run radial distanc function interpolation
	void runRDF();
	// generate the texture for opengl
	void generateTexture();
private:
	// trajectories
	std::vector<std::vector<DPoint3>> _vecTrj;
	std::vector<LabeledPoint> _vecPersonActivity;
	// display index
	GLuint _gllist;                           
};

