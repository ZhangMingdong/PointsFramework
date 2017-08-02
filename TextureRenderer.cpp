#include "TextureRenderer.h"



TextureRenderer::TextureRenderer(int nWidth, int nHeight):_pTexture(NULL)
{
	_nWidth = nWidth;
	_nHeight = nHeight;
	_pTexture = new GLubyte[_nWidth*_nHeight * 4];
}


TextureRenderer::~TextureRenderer()
{

	if (_pTexture) delete[] _pTexture;
}

void TextureRenderer::Draw(float fLeft, float fRight, float fTop, float fBottom) {

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, _uiTexID);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(fLeft, fBottom);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(fRight, fBottom);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(fRight, fTop);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(fLeft, fTop);

	glEnd();

	glDisable(GL_TEXTURE_2D);
}


void TextureRenderer::SetTextureData(int nIndex, GLubyte arrData[4]) {
	for (size_t i = 0; i < 4; i++)
	{
		_pTexture[nIndex * 4 + i] = arrData[i];
	}
}
void TextureRenderer::GenerateTexture() {

	glGenTextures(1, &_uiTexID);
	glBindTexture(GL_TEXTURE_2D, _uiTexID);
	// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _nWidth, _nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, _pTexture);
}