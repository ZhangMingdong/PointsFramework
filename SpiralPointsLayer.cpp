#include "SpiralPointsLayer.h"
#include <QGLWidget>
#include <gl/GLU.h>


SpiralPointsLayer::SpiralPointsLayer()
{
	for (size_t i = 0; i < _nClass; i++)
	{
		std::vector<Point> vecPoint;
		for (size_t j = 0; j < _nPointPerClass; j++)
		{
			double r = j / (double)_nPointPerClass;
			double t = i * 4 + 4.0 / _nPointPerClass + rand() / RAND_MAX*0.2;
			vecPoint.push_back(DPoint3(r*sin(t), r*cos(t), 0));
		}
		_vecPoints.push_back(vecPoint);
	}
}


SpiralPointsLayer::~SpiralPointsLayer()
{
}

void SpiralPointsLayer::Draw() {
	double colors[3][3] = {
		{1,0,0}
		,{0,1,0}
		,{0,0,1}
	};
	glPointSize(2.0f);
	glBegin(GL_POINTS);
	for (size_t i = 0; i < _nClass; i++)
	{

		glColor3dv(colors[i]);
		for (size_t j = 0; j < _nPointPerClass; j++)
		{
			glVertex3d(_vecPoints[i][j].x, _vecPoints[i][j].y, 0);

		}
	}
	glEnd();
}
void SpiralPointsLayer::Clear() {

}
void SpiralPointsLayer::UpdateLayer() {

}