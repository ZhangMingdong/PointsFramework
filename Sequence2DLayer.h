#pragma once
#include "ILayer.h"
#include <vector>
#include "MathTypes.h"
#include <QGLWidget>
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
private:
	// 2D sequence
	std::vector<Point> _sequence;
	std::vector<Point> _sequenceResult;

	// texture data and id
	GLubyte* _dataTexture;
	GLuint texID[1];
};

