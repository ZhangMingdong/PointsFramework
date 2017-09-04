#include "SingleNormalPointsLayer.h"

#include <QGLWidget>
#include <gl/GLU.h>
#include "tnt_array1d.h"
#include "tnt_array2d.h"

#include "jama_eig.h"

#include "MathFunction.h"
#include "RBFInterpolator.h"

#include "ColorMap.h"

#include <algorithm>


const double c_dbK = 1.0 / sqrt(PI2d);
inline double KernelFun(double para) {
	return c_dbK*exp(-para*para / 2.0);
}



SingleNormalPointsLayer::SingleNormalPointsLayer(int number):_pTRenderer(NULL)
, _pCEllipse(NULL)
{
	_nSourceLen = number;
}


SingleNormalPointsLayer::~SingleNormalPointsLayer()
{
	if (_pTRenderer) delete[] _pTRenderer;
	if (_pCEllipse) delete[] _pCEllipse;
}

void SingleNormalPointsLayer::Draw() {

	ColorMap* colormap = ColorMap::GetInstance();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // GL_ONE_MINUS_SRC_ALPHA
	int nLen = _points.size();
	// draw points
	glBegin(GL_POINTS);
	if (_sequence.empty()) {
		glColor4f(0, 1, 1, 1);
		for (int i = 0; i<nLen; i++)
		{
			glVertex3d(_points[i].x, _points[i].y, 0);
		}
	}
	else {
		for each (DPoint3 pt in _sequence)
		{
			cout << pt.z << endl;
			MYGLColor color = colormap->GetColor(pt.z);

			GLubyte bufData[4] = { color._rgb[0]
				, color._rgb[1]
				, color._rgb[2]
				, (GLubyte)255 };
			glColor4ubv(bufData);
			glVertex3d(pt.x, pt.y, 0);
		}
	}


	glEnd();

	if (_bShowBackground) {
		float _fLeft = -_dbRadius;
		float _fRight = _dbRadius;
		float _fBottom = -_dbRadius;
		float _fTop = _dbRadius;
		_pTRenderer->Draw(_fLeft, _fRight, _fTop, _fBottom);
	}
}


void SingleNormalPointsLayer::generateTextureByConfidenceEllipse() {
	// 1.create confidence ellipse
	_pCEllipse = new ConfidenceEllipse(_points);

	ColorMap* colormap = ColorMap::GetInstance();
	// 2.generate texture
	_pTRenderer = new TextureRenderer(_nResultLen, _nResultLen);
	for (size_t i = 0; i < _nResultLen; i++)
	{
		double x = -_dbRadius + i*0.01;
		for (size_t j = 0; j < _nResultLen; j++)
		{
			double y = -_dbRadius + j*0.01;

			double alpha = _pCEllipse->CalculateAlpha(DPoint3(x, y, 0));
			double dbOpacity = 1 - alpha / 5.0;
			if (dbOpacity < 0) dbOpacity = 0;


			int nIndex = j*_nResultLen + i;

			//GLubyte bufData[4] = { (GLubyte)255, (GLubyte)255, 0, (GLubyte)(dbOpacity * 255) };

			MYGLColor color = colormap->GetColor(dbOpacity*6);
			GLubyte bufData[4] = { color._rgb[0]
				, color._rgb[1]
				, color._rgb[2]
				, (GLubyte)255 };

			_pTRenderer->SetTextureData(nIndex, bufData);
		}
	}
	_pTRenderer->GenerateTexture();
}

void SingleNormalPointsLayer::generateTextureByRBF() {
	_sequence.clear();
	vector<DPoint3> sequenceResult;
	// set the value of z to 1 of the sample points
	for each (Point pt in _points)
	{
		_sequence.push_back(DPoint3(pt.x, pt.y, pt.z));
	}

	// 1.build the interpolator
	RBFInterpolator _interpolator;
	_interpolator.Build(_sequence, funPhi,_dbPhiRadius);

	ColorMap* colormap = ColorMap::GetInstance(ColorMap::CP_RainBow);

	// 2.generate result
	_pTRenderer = new TextureRenderer(_nResultLen, _nResultLen);
	// 2.generate result
	double dbStep = _dbRadius * 2 / _nResultLen;
	for (size_t i = 0; i < _nResultLen; i++)
	{
		for (size_t j = 0; j < _nResultLen; j++)
		{
			double x = -_dbRadius + dbStep*j;
			double y = -_dbRadius + dbStep*i;

			int nIndex = i*_nResultLen + j;

			/*
			// not use color map
			GLubyte bufData[4] = { (GLubyte)255
				, 0
				, 0
				, (GLubyte)(_interpolator.Calculate(x, y)*255) };
				*/
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


void SingleNormalPointsLayer::generateTextureByKDE() {

	for each (Point pt in _points)
	{
		_sequence.push_back(DPoint3(pt.x, pt.y, 1));
	}

	ColorMap* colormap = ColorMap::GetInstance();

	// 2.generate result
	_pTRenderer = new TextureRenderer(_nResultLen, _nResultLen);
	// 2.generate result
	double dbStep = _dbRadius * 2 / _nResultLen;
	int nLen = _points.size();
	for (size_t i = 0; i < _nResultLen; i++)
	{
		for (size_t j = 0; j < _nResultLen; j++)
		{
			double x = -_dbRadius + dbStep*j;
			double y = -_dbRadius + dbStep*i;

			int nIndex = i*_nResultLen + j;
			double dbDensity = 0.0;
			double b = .08;
			// calculate the effection for each point
			for each (Point pt in _points)
			{
				double disX = x - pt.x;
				double disY = y - pt.y;
				double dis2 = disX*disX + disY*disY;
				double kb = b*KernelFun(dis2/b);
				dbDensity += kb;
			}
			dbDensity /= nLen;

//			dbDensity *= 300;	// scaled by 6
			dbDensity = sqrt(dbDensity * 10000);
//			if(dbDensity>0.00001) cout << dbDensity << endl;
			MYGLColor color = colormap->GetColor(dbDensity);
			GLubyte bufData[4] = { color._rgb[0]
				, color._rgb[1]
				, color._rgb[2]
				, (GLubyte)255 };
			_pTRenderer->SetTextureData(nIndex, bufData);

		}
	}
	_pTRenderer->GenerateTexture();
}

struct Pair {
	int _nDepth;
	int _nSequence;
	Pair(int nDepth, int nSequence) :_nDepth(nDepth), _nSequence(nSequence) {}
};

bool Compair(Pair p1, Pair p2) {
	return p1._nDepth < p2._nDepth;
}

void SingleNormalPointsLayer::generateTextureBySimplicialDepth() {
	_sequence.clear();
	vector<DPoint3> sequenceResult;
	int nMinDepth = 10000;
	int nMaxDepth = 0;

	vector<Pair> vecDepth;
	// set the value of z to 1 of the sample points
	int nSequence = 0;
	for each (Point pt in _points)
	{
		int nDepth = 0;
		for each (Point pt1 in _points)
		{
			for each (Point pt2 in _points)
			{
				for each (Point pt3 in _points)
				{
					if (MathMethod::PointinTriangle(pt1, pt2, pt3, pt)) nDepth++;
				}
			}
		}
		vecDepth.push_back(Pair(nDepth, nSequence++));
		_sequence.push_back(DPoint3(pt.x, pt.y, 0));
	}
	sort(vecDepth.begin(), vecDepth.end(), Compair);
	int nLevel = 0;
	int nDepth = -1;
	for each (Pair p in vecDepth)
	{
		if (p._nDepth > nDepth) {
			nLevel++;
			nDepth = p._nDepth;
		}
		_sequence[p._nSequence].z = nLevel;
	}
	for (size_t i = 0,length=_sequence.size(); i < length; i++)
	{
		_sequence[i].z = _sequence[i].z / nLevel*6;
//		cout << sequence[i].z << endl;
	}

	// 1.build the interpolator
	RBFInterpolator _interpolator;
	_interpolator.Build(_sequence, funPhi);

	ColorMap* colormap = ColorMap::GetInstance();

	// 2.generate result
	_pTRenderer = new TextureRenderer(_nResultLen, _nResultLen);
	// 2.generate result
	double dbStep = _dbRadius * 2 / _nResultLen;
	for (size_t i = 0; i < _nResultLen; i++)
	{
		for (size_t j = 0; j < _nResultLen; j++)
		{
			double x = -_dbRadius + dbStep*j;
			double y = -_dbRadius + dbStep*i;

			int nIndex = i*_nResultLen + j;

			/*
			// not use color map
			GLubyte bufData[4] = { (GLubyte)255
			, 0
			, 0
			, (GLubyte)(_interpolator.Calculate(x, y)*255) };
			*/
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

// initialization
void SingleNormalPointsLayer::Initialize() {
	generatePoints();
	doInterpolation();
}


void SingleNormalPointsLayer::SetMethod(int nMethod) {
	_nMethod = nMethod;
	doInterpolation();
}


void SingleNormalPointsLayer::SetSource(int nSource) {
	_nSource = nSource;
	generatePoints();
	doInterpolation();
}

void SingleNormalPointsLayer::SetRadius(double r)
{
	_dbPhiRadius = r;
	doInterpolation();
};

// generate source points
void SingleNormalPointsLayer::generatePoints() {
	_points.clear();
	switch (_nSource) {
	case 0:
		GenerateNormalPoints(_points, _nSourceLen, 0, 0, .4, .2, .2, .4);
		break;
	case 1:
		generateDataset1();
		break;
	case 2:
		generateDataset2();
		break;
	case 3:
		generateDataset3();
		break;
	case 4:
		generateDataset4();
		break;
	case 5:
		generateDataset5();
		break;
	}
	
}

// do interpolation
void SingleNormalPointsLayer::doInterpolation() {
	_sequence.clear();
	delete _pTRenderer;
	switch (_nMethod) {
	case 0:
		generateTextureByConfidenceEllipse();
		break;
	case 1:
		generateTextureByRBF();
		break;
	case 2:
		generateTextureBySimplicialDepth();
		break;
	case 3:
		generateTextureByKDE();
		break;
	case 4:
		generateTextureByKDE_LinearKernel();
		break;
	}
}


void SingleNormalPointsLayer::generateDataset1() {
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
		_points.push_back(seq[i]);
	}
}



void SingleNormalPointsLayer::generateDataset2() {
	//*
	double dbValue = 9.0;
	// unique value with new position
	DPoint3 seq[4] = {
		DPoint3(-1.5, -1.5, dbValue)
		,DPoint3(1.5, 1.5, dbValue)
		,DPoint3(-1.5,1.5, dbValue)
		,DPoint3(1.5,-1.5, dbValue)
	};
	//*/
	int nLen = 4;
	for (size_t i = 0; i < nLen; i++)
	{
		_points.push_back(seq[i]);
	}
}


void SingleNormalPointsLayer::generateDataset3() {
	double dbStep = .1;
	int nLen = 10;
	double x = -1.5;
	double y = 0;
	for (size_t i = 0; i < nLen; i++)
	{
		_points.push_back(DPoint3(x,y,0));
		x += dbStep;
		dbStep *= 1.3;
	}
}

void SingleNormalPointsLayer::generateDataset4() {
	GenerateRandomPoints(_points, 50, .8, .5, .5);

	GenerateRandomPoints(_points, 50, -.8, .5, .2);

	GenerateRandomPoints(_points, 10, 0, 0, 1);
}

void SingleNormalPointsLayer::generateDataset5() {
	//*
	double dbValue = 0;
	double dbValue1 = 6.0;
	double dbValue2 = 4.0;
	double dbValue3 = 3.0;
	// unique value with new position
	DPoint3 seq[6] = {
		DPoint3(-1,1,dbValue + dbValue1)
		,DPoint3(-1,-1,dbValue - dbValue1)
		,DPoint3(1,1,dbValue + dbValue2)
		,DPoint3(1,-1,dbValue - dbValue2)
		,DPoint3(1,0,dbValue + dbValue3)
		,DPoint3(-1,0,dbValue - dbValue3)
	};
	//*/
	int nLen = 6;
	for (size_t i = 0; i < nLen; i++)
	{
		_points.push_back(seq[i]);
	}
}


void SingleNormalPointsLayer::generateTextureByKDE_LinearKernel() {

	for each (Point pt in _points)
	{
		_sequence.push_back(DPoint3(pt.x, pt.y, 1));
	}

	ColorMap* colormap = ColorMap::GetInstance();

	// 2.generate result
	_pTRenderer = new TextureRenderer(_nResultLen, _nResultLen);
	// 2.generate result
	double dbStep = _dbRadius * 2 / _nResultLen;
	int nLen = _points.size();
	for (size_t i = 0; i < _nResultLen; i++)
	{
		for (size_t j = 0; j < _nResultLen; j++)
		{
			double x = -_dbRadius + dbStep*j;
			double y = -_dbRadius + dbStep*i;

			int nIndex = i*_nResultLen + j;
			double dbDensity = 0.0;
			double b = 2;
			int nSampleLen = 10;
			// calculate the effection for each point
			for (size_t k = 1; k < nLen; k++)
			{
				for (size_t l = 0; l < nSampleLen; l++)
				{
					double dbSampleX = (_points[k].x*l + _points[k - 1].x*(nSampleLen - l)) / nSampleLen;
					double dbSampleY = (_points[k].y*l + _points[k - 1].y*(nSampleLen - l)) / nSampleLen;

					double disX = x - dbSampleX;
					double disY = y - dbSampleY;
					double dis2 = disX*disX + disY*disY;
					double kb = b*KernelFun(dis2 / b);
					dbDensity += kb;
				}
			}

			dbDensity /= ((nLen - 1)*nSampleLen);

			dbDensity *= 20;	// scaled by 20
							//			if(dbDensity>0.00001) cout << dbDensity << endl;
			MYGLColor color = colormap->GetColor(dbDensity);
			GLubyte bufData[4] = { color._rgb[0]
				, color._rgb[1]
				, color._rgb[2]
				, (GLubyte)255 };
			_pTRenderer->SetTextureData(nIndex, bufData);

		}
	}
	_pTRenderer->GenerateTexture();
}