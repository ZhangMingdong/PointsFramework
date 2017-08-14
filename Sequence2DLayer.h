#pragma once
#include "ILayer.h"
#include <vector>
#include "MathTypes.h"
#include <QGLWidget>
#include "RBFInterpolator.h"
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
private:
	// 2D sequence
	std::vector<DPoint3> _sequence;
	std::vector<DPoint3> _sequenceResult;

	// texture data and id
	GLubyte* _dataTexture;
	GLuint texID[1];
	// length of the result
	int _nResultLen = 100;

	RBFInterpolator _interpolator;
protected:
	// run radial distanc function interpolation
	void runRDF();
	// generate the texture for opengl
	void generateTexture();
};

