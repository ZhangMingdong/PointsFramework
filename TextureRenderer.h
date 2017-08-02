#pragma once
#include <QGLWidget>
/*
	class used to render a rectangular with texture
	mingdong
	2017/07/31
*/
class TextureRenderer
{
public:
	TextureRenderer(int nWidth, int nHeight);
	virtual ~TextureRenderer();
protected:
	// texture data and id
	GLubyte* _pTexture;
	GLuint _uiTexID;
	int _nWidth;
	int _nHeight;
public:
	// draw the texture
	void Draw(float fLeft,float fRight,float fTop, float fBottom);
	// set the data of given index
	void SetTextureData(int nIndex, GLubyte arrData[4]);
	// generate the texture
	void GenerateTexture();


};

