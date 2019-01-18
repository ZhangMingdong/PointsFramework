#include "Sequence2DLayer.h"

#include "ColorMap.h"
#include "tnt_array1d.h"
#include "tnt_array2d.h"

#include "jama_eig.h"

#include <QGLWidget>
#include <gl/GLU.h>
#include <MathTypes.hpp>
#include "TextureRenderer.h"

#include <fstream>

#include <QDebug>


bool g_bShowData = true;
bool g_bShowPersonActivity = true;

Sequence2DLayer::Sequence2DLayer()
{

}

Sequence2DLayer::~Sequence2DLayer()
{
	if (_pTRenderer) delete[] _pTRenderer;

	if (glIsList(_gllist))
		glDeleteLists(_gllist, 1);
}

void Sequence2DLayer::Sequence2DLayer::Draw() {
	if (g_bShowData) {

		if (glIsList(_gllist))
			glCallList(_gllist);
		return;
	}
	// draw 2d sequence
	int nSeqLen2D = _sequence.size();
	if (nSeqLen2D>0)
	{
		ColorMap* colormap = ColorMap::GetInstance();

		// draw points
		glPointSize(10);
		glBegin(GL_POINTS);
		for (size_t i = 0; i < nSeqLen2D; i++)
		{
			MYGLColor color = colormap->GetColor(_sequence[i].z);
			glColor3d(color._rgb[0] / 255.0, color._rgb[1] / 255.0, color._rgb[2] / 255.0);
			glVertex3d(_sequence[i].x, _sequence[i].y, 0);

		}
		glEnd();


		// draw field
		if (_pSetting->_bShowBg) {
			float _fLeft = -_pSetting->_dbRadius;
			float _fRight = _pSetting->_dbRadius;
			float _fBottom = -_pSetting->_dbRadius;
			float _fTop = _pSetting->_dbRadius;
			_pTRenderer->Draw(_fLeft, _fRight, _fTop, _fBottom);
		}
	}
}

vector<string> split(string strtem, char a)
{
	vector<string> strvec;

	string::size_type pos1, pos2;
	pos2 = strtem.find(a);
	pos1 = 0;
	while (string::npos != pos2)
	{
		strvec.push_back(strtem.substr(pos1, pos2 - pos1));

		pos1 = pos2 + 1;
		pos2 = strtem.find(a, pos1);
	}
	strvec.push_back(strtem.substr(pos1));
	return strvec;
}

void Sequence2DLayer::Initialize() {
	if (g_bShowPersonActivity) {
		// read trajectories
		ifstream trjInput("F:/Data/Person Activity/ConfLongDemo_JSI.txt");
		float colors[3][3] = {
			{.97,.62,.2}
			,{.68,.22,.27}
			,{.13,.37,.67}
		};

		string strLine;
		while (getline(trjInput, strLine)) {
			vector<string> vStr = split(strLine, ',');
			int nLabel = -1;
			if (vStr[7] == "sitting") nLabel = 0;
			else if (vStr[7] == "standing up from lying") nLabel = 1;
			else if (vStr[7] == "walking") nLabel = 2;
			if (nLabel > -1) {
				_vecPersonActivity.push_back(LabeledPoint(stod(vStr[4]), stod(vStr[5]), nLabel));
			}
		}
		// initialize the display list

		_gllist = glGenLists(1);	// generate the display lists
		glNewList(_gllist, GL_COMPILE);
		glBegin(GL_POINTS);
		/*
		for (size_t i = 0, length = _vecPersonActivity.size(); i < length; i++)
		{
			glColor3fv(colors[_vecPersonActivity[i]._nLabel]);
			glVertex2f(_vecPersonActivity[i]._arrCoord[0], _vecPersonActivity[i]._arrCoord[1]);
		}*/
		for (int label = 2; label >=0; label--) {
			glColor3fv(colors[label]);
			for (size_t i = 0, length = _vecPersonActivity.size(); i < length; i++)
			{
				if(_vecPersonActivity[i]._nLabel==label)
					glVertex2f(_vecPersonActivity[i]._arrCoord[0], _vecPersonActivity[i]._arrCoord[1]);
			}
		}
		glEnd();
		glEndList();
		return;

	}
	if (g_bShowData) {
		// read trajectories
		ifstream trjInput("../data/trajectory.txt");
		int nLen;
		trjInput >> nLen;
		qDebug() << nLen;
		nLen = 1000;
		for (size_t i = 0; i < nLen; i++)
		{
			vector<DPoint3> trj;
			int l;
			int id;
			trjInput >> id >> l;
			for (size_t j = 0; j < l; j++)
			{
				int t;
				double x, y;
				trjInput >> t >> x >> y;
				trj.push_back(DPoint3(x / 10000.0, y / 10000.0 - 6, 0));
			}
			_vecTrj.push_back(trj);
		}

		// initialize the display list

		_gllist = glGenLists(1);	// generate the display lists
		glNewList(_gllist, GL_COMPILE);
		glColor4f(0, 0, 1, .5);
		for (size_t i = 0, length = _vecTrj.size(); i < length; i++)
		{
			glBegin(GL_LINE_STRIP);
			for (size_t j = 0, len2 = _vecTrj[i].size(); j < len2; j++)
			{
				glVertex2d(_vecTrj[i][j].x, _vecTrj[i][j].y);
			}
			glEnd();
		}
		glEndList();
		return;

	}


	_sequence.clear();
	// 0.generation

	/*	
	// random setted data set
	DPoint3 seq[10] = {
		DPoint3(-1.5, -1.5, 2)
		,DPoint3(-1.6, -0.6, 6)
		,DPoint3(-1.2, 0, 10)
		,DPoint3(-0.8, .3, 3)
		,DPoint3(-0.1, .5, 8)
		,DPoint3(-0.0, .7, 14)
		,DPoint3(0.4, .9, 6)
		,DPoint3(0.6, .5, 5)
		,DPoint3(1.2, .3, 2)
		,DPoint3(1.6, .0, 9)
	};
	*/

	/*
	// unique value
	DPoint3 seq[10] = {
		DPoint3(-1.5, -1.5, 6)
		,DPoint3(-1.6, -0.6, 6)
		,DPoint3(-1.2, 0, 6)
		,DPoint3(-0.8, .3, 6)
		,DPoint3(-0.1, .5, 6)
		,DPoint3(-0.0, .7, 6)
		,DPoint3(0.4, .9, 6)
		,DPoint3(0.6, .5, 6)
		,DPoint3(1.2, .3, 6)
		,DPoint3(1.6, .0, 6)
	};
	*/

	//*
	double dbValue = 9.0;
	// unique value with new position
	DPoint3 seq[10] = {
		DPoint3(-1.5, 0, dbValue)
		,DPoint3(-0.2, 0, dbValue)
		,DPoint3(0.2, 0, dbValue)
		,DPoint3(1.5, 0, dbValue)
		,DPoint3(0, 1, dbValue)
		,DPoint3(0, -1, dbValue)
		,DPoint3(-1, .8, dbValue)
		,DPoint3(1, .8, dbValue)
		,DPoint3(-1, -.8, dbValue)
		,DPoint3(1, -.8, dbValue)
	};
	//*/
	int nLen = 10;
	for (size_t i = 0; i < nLen; i++)
	{
		_sequence.push_back(seq[i]);
	}
	runRDF();

	// 3.generate texture
	generateTexture();
}

void Sequence2DLayer::runRDF() {
	// 1.build the interpolator
	_interpolator.Build(_sequence, funPhi);
}

void Sequence2DLayer::generateTexture() {
	_pTRenderer = new TextureRenderer(_pSetting->_nResultLen, _pSetting->_nResultLen);
	ColorMap* colormap = ColorMap::GetInstance();
	// 2.generate result
	double dbStep = _pSetting->_dbRadius * 2 / _pSetting->_nResultLen;
	for (size_t i = 0; i < _pSetting->_nResultLen; i++)
	{
		for (size_t j = 0; j < _pSetting->_nResultLen; j++)
		{
			double x = -_pSetting->_dbRadius + dbStep*j;
			double y = -_pSetting->_dbRadius + dbStep*i;

			int nIndex = i*_pSetting->_nResultLen + j;
			MYGLColor color = colormap->GetColor(_interpolator.Calculate(x, y));
			GLubyte bufData[4] = { color._rgb[0]
				, color._rgb[1]
				, color._rgb[2]
				, (GLubyte)255 };
			_pTRenderer->SetTextureData(nIndex, bufData);

		}
	}
	_pTRenderer->GenerateTexture();

}

void Sequence2DLayer::SetMethod(int nMethod) {
	_nMethod = nMethod;
}