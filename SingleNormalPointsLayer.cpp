#include "SingleNormalPointsLayer.h"

#include <QGLWidget>
#include <gl/GLU.h>
#include "tnt_array1d.h"
#include "tnt_array2d.h"

#include "jama_eig.h"

#include "MathFunction.h"
#include "RBFInterpolator.h"
#include "AHCClustering.h"
#include "KMeansClustering.h"
#include "DBSCANClustering.h"
#include "OPTICSClustering.h"

#include "ColorMap.h"

#include <algorithm>
#include <fstream>

#include <QDebug>


const double c_dbK = 1.0 / sqrt(PI2d);
const double c_dbEpsilon = 1;
inline double KernelFun(double para) {
	return c_dbK*exp(-para*para / 2.0/c_dbEpsilon/c_dbEpsilon);
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
	glPointSize(_pSetting->_dbPointSize);

	if (_pSetting->_bClustering)
	{
		if (_clusteredPoints.size()>0)
		{
			RGBAf arrColors[4] = {
				RGBAf(1, 0, 0, 1)
				,RGBAf(0, 1, 0, 1)
				,RGBAf(0, 0, 1, 1)
				,RGBAf(1, 0, 1, 1)
			};
			for (size_t i = 0; i < _nCluster; i++)
			{
				int x = i * 100;
				int r = x < 256 ? 255 - x : x < 512 ? 0 : x - 512;
				int g = x < 256 ? x : x < 512 ? 512 - x : 0;
				int b = x < 256 ? 0 : x < 512 ? x - 256 : 768 - x;
				glColor3f(r / 255.0, g / 255.0, b / 255.0);

				//glColor4f(arrColors[i].r, arrColors[i].g, arrColors[i].b, arrColors[i].a);
				glBegin(GL_POINTS);
				for (size_t j = 0; j<_clusteredPoints[i].size(); j++)
				{
					glVertex3d(_clusteredPoints[i][j].x, _clusteredPoints[i][j].y, 0);
				}
				glEnd();
			}
		}
		int nLen = _points.size();
		// draw points
		glBegin(GL_POINTS);
		glColor4f(.2, .2, .2, 1);
		for (int i = 0; i<nLen; i++)
		{
			glVertex3d(_points[i].x, _points[i].y, 0);
		}

		glEnd();
	}
	else 
	{
		ColorMap* colormap = ColorMap::GetInstance();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE); // GL_ONE_MINUS_SRC_ALPHA
		int nLen = _points.size();
		// draw points
		glBegin(GL_POINTS);
		if (_pSetting->_bSD) {
			for each (DPoint3 pt in _points)
			{
				double dbScale = 1.0;// 8.0;
//				cout << pt.z << endl;
				MYGLColor color = colormap->GetColor(pt.z*dbScale);

				GLubyte bufData[4] = { color._rgb[0]
					, color._rgb[1]
					, color._rgb[2]
					, (GLubyte)255 };
				glColor4ubv(bufData);
				glVertex3d(pt.x, pt.y, 0);
			}

		}
		else {
			glColor4f(0, 1, 1, 1);
			for (int i = 0; i<nLen; i++)
			{
				glVertex3d(_points[i].x, _points[i].y, 0);
			}
		}

		glEnd();
	}


	if (_pSetting->_bShowBg) {
		float _fLeft = -_pSetting->_dbRadius;
		float _fRight = _pSetting->_dbRadius;
		float _fBottom = -_pSetting->_dbRadius;
		float _fTop = _pSetting->_dbRadius;
		_pTRenderer->Draw(_fLeft, _fRight, _fTop, _fBottom);
	}


}

void SingleNormalPointsLayer::generateTextureByConfidenceEllipse() {
	// 1.create confidence ellipse
	_pCEllipse = new ConfidenceEllipse(_points);

	ColorMap* colormap = ColorMap::GetInstance();
	// 2.generate texture
	_pTRenderer = new TextureRenderer(_pSetting->_nResultLen, _pSetting->_nResultLen);
	for (size_t i = 0; i < _pSetting->_nResultLen; i++)
	{
		double x = -_pSetting->_dbRadius + i*0.01;
		for (size_t j = 0; j < _pSetting->_nResultLen; j++)
		{
			double y = -_pSetting->_dbRadius + j*0.01;

			double alpha = _pCEllipse->CalculateAlpha(DPoint3(x, y, 0));
			cout << alpha << endl;
			double dbOpacity = 1 - alpha / 5.0;
			if (dbOpacity < 0) dbOpacity = 0;


			int nIndex = j*_pSetting->_nResultLen + i;

			GLubyte bufData[4] = { (GLubyte)255, (GLubyte)255, 0, (GLubyte)(dbOpacity * 255) };

//			MYGLColor color = colormap->GetColor(dbOpacity*6);
//			GLubyte bufData[4] = { color._rgb[0]
//				, color._rgb[1]
//				, color._rgb[2]
//				, (GLubyte)255 };

			_pTRenderer->SetTextureData(nIndex, bufData);
		}
	}
	_pTRenderer->GenerateTexture();
}

void SingleNormalPointsLayer::generateTextureByRBF() {
	// 0.set the value of z to 1 of the sample points
	vector<DPoint3> sequence;
	for each (DPoint3 pt in _points)
	{
		sequence.push_back(DPoint3(pt.x, pt.y, 1));
	}

	// 1.build the interpolator
	RBFInterpolator _interpolator;
	_interpolator.Build(sequence, funPhi, _pSetting->_dbPhiRadius);

	ColorMap* colormap = ColorMap::GetInstance(ColorMap::CP_RainBow);

	// 2.create a new instance for the renderer
	_pTRenderer = new TextureRenderer(_pSetting->_nResultLen, _pSetting->_nResultLen);
	// 3.generate the renderer
	double dbStep = _pSetting->_dbRadius * 2 / _pSetting->_nResultLen;
	for (size_t i = 0; i < _pSetting->_nResultLen; i++)
	{
		for (size_t j = 0; j < _pSetting->_nResultLen; j++)
		{
			double x = -_pSetting->_dbRadius + dbStep*j;
			double y = -_pSetting->_dbRadius + dbStep*i;
			// for each grid point(x,y)
			int nIndex = i*_pSetting->_nResultLen + j;
			double dbResult = _interpolator.Calculate(x, y);
			MYGLColor color = colormap->GetColor(dbResult);
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
	ColorMap* colormap = ColorMap::GetInstance();

	// 1.Create an instance of the texture renderer
	_pTRenderer = new TextureRenderer(_pSetting->_nResultLen, _pSetting->_nResultLen);
	// 2.generate texture data
	double dbStep = _pSetting->_dbRadius * 2 / _pSetting->_nResultLen;
	int nLen = _points.size();
	for (size_t i = 0; i < _pSetting->_nResultLen; i++)
	{
		for (size_t j = 0; j < _pSetting->_nResultLen; j++)
		{
			double x = -_pSetting->_dbRadius + dbStep*j;
			double y = -_pSetting->_dbRadius + dbStep*i;

			// for every grid points (x,y)

			int nIndex = i*_pSetting->_nResultLen + j;
			double dbDensity = 0.0;
			double B = .05;
			// calculate the effection for each point
			for each (DPoint3 pt in _points)
			{
				double dbX = x - pt.x;
				double dbY = y - pt.y;
				double dbR = sqrt(dbX*dbX + dbY*dbY);
				double kb = KernelFun(dbR/B)/B;
				dbDensity += kb;
			}
			dbDensity /= nLen;
			dbDensity *= 20;
//			dbDensity *= 300;	// scaled by 6
//			dbDensity = sqrt(dbDensity * 100);
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

void SingleNormalPointsLayer::calculateSimplicialSepth(std::vector<DPoint3>& points) {

	vector<DPoint3> sequenceResult;
	int nMinDepth = 10000;
	int nMaxDepth = 0;

	// store the index and depth of the sample points
	vector<Pair> vecDepth;

	// 1.calculate depth value for every sample point
	int nSequence = 0;
	for each (DPoint3 pt in points)
	{
		int nDepth = 0;
		for each (DPoint3 pt1 in points)
		{
			for each (DPoint3 pt2 in points)
			{
				for each (DPoint3 pt3 in points)
				{
					if (MathMethod::PointinTriangle(pt1, pt2, pt3, pt)) nDepth++;
				}
			}
		}
		vecDepth.push_back(Pair(nDepth, nSequence++));
	}
	// 2.sort the pairs according to their depth
	sort(vecDepth.begin(), vecDepth.end(), Compair);
	// 3.generate sequence of the depth
	int nLevel = 0;
	int nDepth = -1;
	for each (Pair p in vecDepth)
	{
		if (p._nDepth > nDepth) {
			nLevel++;
			nDepth = p._nDepth;
		}
		points[p._nSequence].z = nLevel;
	}
	// 4.map the sequce level to 0~1
	for (size_t i = 0, length = points.size(); i < length; i++)
	{
		points[i].z = points[i].z / nLevel;
		//		cout << sequence[i].z << endl;
	}

}

void SingleNormalPointsLayer::generateTextureBySimplicialDepth() {

	// 1.build the interpolator
	RBFInterpolator _interpolator;
	_interpolator.Build(_points, funPhi);

	ColorMap* colormap = ColorMap::GetInstance();

	// 2.generate result
	_pTRenderer = new TextureRenderer(_pSetting->_nResultLen, _pSetting->_nResultLen);
	// 2.generate result
	double dbStep = _pSetting->_dbRadius * 2 / _pSetting->_nResultLen;
	for (size_t i = 0; i < _pSetting->_nResultLen; i++)
	{
		for (size_t j = 0; j < _pSetting->_nResultLen; j++)
		{
			double x = -_pSetting->_dbRadius + dbStep*j;
			double y = -_pSetting->_dbRadius + dbStep*i;

			int nIndex = i*_pSetting->_nResultLen + j;

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
	if(_pSetting->_bInterpolation)
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

// generate source points
void SingleNormalPointsLayer::generatePoints() {
	_points.clear();
	_groupedPoints.clear();
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
	case 6:
		generateDataset6();
		break;
	case 7:
		generateDataset7();
		return;	// never calculate depth data for this dataset
		break;
	case 8:
		generateDataset8();
		return;	// never calculate depth data for this dataset
		break;
	case 9:
		generateDataset9();
		break;
	case 10:
		generateDataset10();
		break;
	}


	// calculate simplicial depth
	if (_pSetting->_bSD)
	{
		if (_groupedPoints.size()>0)
		{
			for (size_t i = 0, length = _groupedPoints.size(); i < length; i++)
			{
				calculateSimplicialSepth(_groupedPoints[i]);
			}
		}
		else
			calculateSimplicialSepth(_points);
	}
	for (size_t i = 0; i < _groupedPoints.size(); i++)
	{
		for (size_t j = 0, length = _groupedPoints[i].size(); j < length; j++)
		{
			_points.push_back(_groupedPoints[i][j]);
		}
	}
	
}

// do interpolation
void SingleNormalPointsLayer::doInterpolation() {
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

void SingleNormalPointsLayer::generateDataset6() {
	double mx = 0;
	double my = 0;
	double vx = .4;
	double vy = .2;

	double dbScale = 1.5;

	int nLen = _nSourceLen / _nCluster;
	for (size_t i = 0; i < _nCluster; i++)
	{
		_groupedPoints.push_back(vector<DPoint3>());
	}


	GenerateNormalPoints(_groupedPoints[0], _nSourceLen, mx - vx * dbScale, my - vy * dbScale, vx, vy);
	GenerateNormalPoints(_groupedPoints[1], _nSourceLen, mx - vx * dbScale, my + vy * dbScale, vx, vy);
	GenerateNormalPoints(_groupedPoints[2], _nSourceLen, mx + vx * dbScale, my + vy * dbScale, vx, vy);
	GenerateNormalPoints(_groupedPoints[3], _nSourceLen, mx + vx * dbScale, my - vy * dbScale, vx, vy);
	_nSourceLen = nLen * _nCluster;


}

void SingleNormalPointsLayer::generateDataset7() {
	ifstream input("variance.txt");
	for (size_t i = 0; i < 65161; i++)
	{
		double x, y, z;
		input >> x >> y >> z;
		if (z>10.0)
		_points.push_back(DPoint3(x/100.0, y/100.0, z));
	}
	cout <<"Number of Points: "<< _points.size() << endl;
}

void SingleNormalPointsLayer::generateDataset8() {
	ifstream input("mean.txt");
	for (size_t i = 0; i < 65161; i++)
	{
		double x, y, z;
		input >> x >> y >> z;
		_points.push_back(DPoint3(x / 100.0, y / 100.0, z));
	}
}

void SingleNormalPointsLayer::generateDataset9() {
	double mx = 0;
	double my = 0;
	double vx = .4;
	double vy = .2;


	_nCluster = 4;
	int nLen = _nSourceLen / _nCluster;


	GenerateNormalPoints(_points, nLen, -.3, 1, .1, .1);
	GenerateNormalPoints(_points, nLen, .3, 1, .1, .1);
	GenerateNormalPoints(_points, nLen, -1, -1, .4, .4);
	GenerateNormalPoints(_points, nLen, 1, -1, .2, .4);
	_nSourceLen = nLen * _nCluster;


}

void SingleNormalPointsLayer::generateDataset10() {
	//*
	double dbValue = 1.0;
	// unique value with new position
	DPoint3 seq[17] = {
		DPoint3(.2, .3, dbValue),
		DPoint3(.2, .4, dbValue),
		DPoint3(.1, .4, dbValue),
		DPoint3(.1, .3, dbValue),
		DPoint3(.2, .2, dbValue),
		DPoint3(.3, .2, dbValue),
		DPoint3(.8, .7, dbValue),
		DPoint3(.8, .6, dbValue),
		DPoint3(.7, .7, dbValue),
		DPoint3(.7, .6, dbValue),
		DPoint3(.8, .5, dbValue),
		DPoint3(10.0, .2, dbValue),
		DPoint3(.8, 2.0, dbValue),
		DPoint3(.8, 1.9, dbValue),
		DPoint3(.7, 1.8, dbValue),
		DPoint3(.7, 1.7, dbValue),
		DPoint3(.8, 2.1, dbValue),
	};
	//*/
	int nLen = 17;
	for (size_t i = 0; i < nLen; i++)
	{
		_points.push_back(seq[i]);
	}



}
void SingleNormalPointsLayer::generateTextureByKDE_LinearKernel() {
	ColorMap* colormap = ColorMap::GetInstance();

	// 2.generate result
	_pTRenderer = new TextureRenderer(_pSetting->_nResultLen, _pSetting->_nResultLen);
	// 2.generate result
	double dbStep = _pSetting->_dbRadius * 2 / _pSetting->_nResultLen;
	int nLen = _points.size();
	for (size_t i = 0; i < _pSetting->_nResultLen; i++)
	{
		for (size_t j = 0; j < _pSetting->_nResultLen; j++)
		{
			double x = -_pSetting->_dbRadius + dbStep*j;
			double y = -_pSetting->_dbRadius + dbStep*i;

			int nIndex = i*_pSetting->_nResultLen + j;
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

void SingleNormalPointsLayer::UpdateLayer() {
	if (_pSetting->_bInterpolation)
		doInterpolation();

	if (_pSetting->_bClustering)
		clustering();
}

void SingleNormalPointsLayer::clustering() {

	bool bWeightedDBSCAN = _pSetting->_bSD;

	CLUSTER::Clustering* pClusterer = NULL;
	switch (_pSetting->_nClusteringMethod)
	{
	case 0:
		_nCluster = 4;
		pClusterer = new CLUSTER::AHCClustering();
		break;
	case 1:
		_nCluster = 4;
		pClusterer = new CLUSTER::KMeansClustering();
		break;
	case 2:
	{
		CLUSTER::DBSCANClustering* pSBSCAN = new CLUSTER::DBSCANClustering();
		pSBSCAN->SetDBSCANParams(_pSetting->_nMinPts, _pSetting->_dbEps, bWeightedDBSCAN);
		pClusterer = pSBSCAN;
		break;
	}
	case 3:
	{
		CLUSTER::OPTICSClustering* pOPTICS = new CLUSTER::OPTICSClustering();
		pOPTICS->SetDBSCANParams(_pSetting->_nMinPts, _pSetting->_dbEps, bWeightedDBSCAN);
		pClusterer = pOPTICS;
		break;
	}
	default:
		return;
		break;
	}

	int number = _points.size();
	int* arrLabel = new int[number];
	double* arrBuf;
	if (bWeightedDBSCAN)
	{

		arrBuf = new double[number * 3];
		for (size_t i = 0; i < number; i++)
		{
			arrBuf[i * 3] = _points[i].x;
			arrBuf[i * 3 + 1] = _points[i].y;
			arrBuf[i * 3 + 2] = _points[i].z;
		}

	}
	else {
		arrBuf = new double[number * 2];
		for (size_t i = 0; i < number; i++)
		{
			arrBuf[i * 2] = _points[i].x;
			arrBuf[i * 2 + 1] = _points[i].y;
		}

	}

	_nCluster = pClusterer->DoCluster(number, 2, _nCluster, arrBuf, arrLabel);


	// 2.generate clusters
	_clusteredPoints.clear();
	for (size_t i = 0; i < _nCluster; i++)
	{
		_clusteredPoints.push_back(std::vector<DPoint3>());
	}
	for (size_t i = 0; i < number; i++)
	{
		//		qDebug() << arrLabel[i];
		if (arrLabel[i] >= 0)
		{
			_clusteredPoints[arrLabel[i]].push_back(_points[i]);

		}
		else
			qDebug() << arrLabel[i];
	}
	qDebug() << _clusteredPoints.size();
	delete pClusterer;
	delete arrBuf;
	delete arrLabel;
}