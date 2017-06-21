#pragma once
#include "ILayer.h"
#include <vector>
#include "MathTypes.h"
#include <QGLWidget>
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
	std::vector<Point> _sequence2D;
	std::vector<Point> _sequenceResult2D;

	// texture data and id
	GLubyte* _dataTexture;
	GLuint texID[1];
};

