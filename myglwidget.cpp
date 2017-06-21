#include "myglwidget.h"
#include "zcalcfuncs.h"
#include <sstream>

#include <math.h>
#include <algorithm>

#include <QWheelEvent>



#include "ILayer.h"

#include <assert.h>
#include <fstream>


using namespace std;

#define	GLAYER_STATE_MOVING			0
#define GLAYER_STATE_RINGING				1
#define GLAYER_STATE_CALCULATING		2

bool debug = false;


MyGLWidget::MyGLWidget(QWidget *parent)
	: QGLWidget(parent)
	, m_bLBtnDown(false)
	, m_bRBtnDown(false)
	, m_bMBtnDown(false)
	, _uiNumberOfPoints(0)
	, _bCalculated(false)
	, _bHandPoint(false)
	, m_sPolyStyle(0)
	, m_pt3Eye(0.0, 0.0, 4.0)
	, m_clearColor(.6, .6, .4, 1.0)
	,_pLayer(0)
{
	_result._nIndex2 = -1;
	_result._dbDis = 0;
	OnCreate();
	SetClearColor(0, 0, 0, 0);
	SetPointSize(2.0f);
	SetLineWidth(1.0f);


		m_dbFovy = 45.0;
		m_dbZNear = 0.0001;
		m_dbZFar = 1500.0;
		m_nClientH = m_nClientW = 1;

}

MyGLWidget::~MyGLWidget()
{
	Release();

}

void MyGLWidget::initializeGL()
{
// 	glewInit();
	// build texture
// 	char* fileName[] = { "resources\\1.png"
// 		, "resources\\2.png"
// 		, "resources\\3.png"
// 		, "resources\\4.png"
// 		, "resources\\5.png"
// 		, "resources\\6.png"
// 	};
// 	for (int i = 0; i < 6; i++)
// 	{
// 		QImage image(fileName[i]);
// 		texWidth = image.width();
// 		texHeight = image.height();
// 		// 	makeCheckImage();
// 		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
// 
// 		glGenTextures(1, texName + i);
// 		glBindTexture(GL_TEXTURE_2D, texName[i]);
// 
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
// 		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight,
// 			0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
// 		glGenerateMipmap(GL_TEXTURE_2D);
// 	}
	// ~build texture
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void MyGLWidget::paintGL(){
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 	// 1.场景布置
// 	glMatrixMode(GL_MODELVIEW);
// 	glLoadIdentity();
// 	glTranslatef(0.0, 0.0, -10.0);
// 	// 2.缩放
// 	glScalef(_fScale, _fScale, _fScale);
// 	// 3.旋转
// 	glRotatef(_nRotateX, 1, 0, 0);
// 	glRotatef(_nRotateY, 0, 1, 0);
// 	glRotatef(_nRotateZ, 0, 0, 1);
// 
// 	// 4.坐标轴绘制
// 	glDisable(GL_TEXTURE_2D);
// 	// axis
// 	glColor3f(1, 0, 0);
// 	glBegin(GL_LINES);
// 	glVertex3f(0, 0, 0);
// 	glVertex3f(10000, 0, 0);
// 	glEnd();
// 	glColor3f(0, 1, 0);
// 	glBegin(GL_LINES);
// 	glVertex3f(0, 0, 0);
// 	glVertex3f(0, 10000, 0);
// 	glEnd();
// 	glColor3f(0, 0, 1);
// 	glBegin(GL_LINES);
// 	glVertex3f(0, 0, 0);
// 	glVertex3f(0, 0, 10000);
// 	glEnd();
// 	// 5.移到中心
// 	glTranslatef(-1.5, -1.5, -1.5);
// 	// 6.魔方绘制
// 	glEnable(GL_TEXTURE_2D);
// 	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
// 	drawMagicCube();

	Draw();


}

void MyGLWidget::resizeGL(int width, int height){
	// 	_nWidth = width;
	// 	_nHeight = height;
// 	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
// 	glMatrixMode(GL_PROJECTION);
// 	glLoadIdentity();
// 	gluLookAt(0, 0, 1.0, 0, 0, 0, 0, 1, 0);
// 	gluPerspective(60.0, (GLfloat)width / (GLfloat)height, .1, 30.0);
	// 	glMatrixMode(GL_MODELVIEW);
	// 	glLoadIdentity();
	// 	glTranslatef(0.0, 0.0, -3.6);
	OnSize(width, height);
}

void MyGLWidget::timerEvent(QTimerEvent* event)
{
	// 	if (_bRotate)
	// 	{
	// 		_nRotate = (_nRotate + 10) % 360;
	// 	}
	updateGL();
}

void MyGLWidget::mousePressEvent(QMouseEvent * event)
{
	OnLButtonDown(event->pos().x(), event->pos().y());
	updateGL();
}
void MyGLWidget::mouseReleaseEvent(QMouseEvent * event)
{
	OnLButtonUp(event->pos().x(), event->pos().y());
	updateGL();

}
void MyGLWidget::mouseMoveEvent(QMouseEvent * event)
{
	OnMouseMove(event->pos().x(), event->pos().y());
	updateGL();

}
void MyGLWidget::wheelEvent(QWheelEvent * event)
{
	// 	int n = event->delta();
	// 	int b = 0;
// 	if (event->delta() > 0)
// 	{
// 		_fScale *= 1.1;
// 	}
// 	else
// 	{
// 		_fScale /= 1.1;
// 	}
	OnMouseWheel(event->delta());
	updateGL();
}
void MyGLWidget::keyPressEvent(QKeyEvent * event)
{
// 	switch (event->key())
// 	{
// 	case 'x':
// 		_nRotateX = (_nRotateX + 10) % 360;
// 
// 		break;
// 	case 'y':
// 		_nRotateY = (_nRotateY + 10) % 360;
// 		break;
// 	case 'z':
// 		_nRotateZ = (_nRotateZ + 10) % 360;
// 		break;
// 	default:
// 		break;
// 	}
	updateGL();
}

void MyGLWidget::Release()
{
	OnDestroy();
}

void MyGLWidget::Draw()
{
	SetColor(RGBAf(0, 1, 1, 1));
	OnPreRenderScene();
	Transform();
	SetPolygonStyle(1);		//fill
	// 绘制坐标系
	// 	drawCoords();


	if (_pLayer)
		_pLayer->Draw();

	// draw points
	for (int i = 0; i<_uiNumberOfPoints; i++)
	{
		DrawPoint(_points[i]);
	}
	// draw the eclipse
	SetColor(RGBAf(1, 0, 0, 1));
	for (int i = 0; i<_pointsResult.size(); i++)
	{
		DrawPoint(_pointsResult[i]);
	}

	// draw the result
	if (_bCalculated&&_uiNumberOfPoints>0)
	{
		SetColor(RGBAf(1, 0, 0, 1));
		DrawLine(_points[_result._nIndex1], _points[_result._nIndex2]);
		double dbRadius = _result._dbDis * 2;
		DrawCircle(_points[_result._nIndex1], dbRadius);
		if (dbRadius<0.01)
		{
			DrawCircle(_points[_result._nIndex1], 0.1);
		}
	}

	ReTransform();
	OnPostRenderScene();
}

void MyGLWidget::drawCoords()
{
	//坐标轴
	SetColor(RGBAf(0, 0, 0, 1));
	// 	SetLineWidth(3);
	DrawLine3D(DPoint3(0, 0, 0), DPoint3(1000, 0, 0));
	DrawLine3D(DPoint3(0, 0, 0), DPoint3(0, 1000, 0));
	DrawLine3D(DPoint3(0, 0, 0), DPoint3(0, 0, 1000));
	//同心圆
	for (int i = 1; i<10; i++)
	{
		DrawCircle(DPoint3(0, 0, 0), i / 10.0);
	}
}
//////////////////////////////////////////////////////////////////////////
void MyGLWidget::OnLButtonDown(int x, int y)
{
	m_bLBtnDown = true;
	m_ptLBtnDown.x = x;
	m_ptLBtnDown.y = y;
	m_ptMouseCurrent.x = x;
	m_ptMouseCurrent.y = y;

	if (_bHandPoint)
	{
		DPoint3 pt1;
		ScreenToWorld(m_ptLBtnDown, pt1);
		_points.push_back(pt1);
		_uiNumberOfPoints++;
	}
}
void MyGLWidget::OnLButtonUp(int x, int y)
{
	switch (m_nState)
	{
	case GLAYER_STATE_RINGING:
	{
								 DPoint3 pt1, pt2;
								 ScreenToWorld(m_ptLBtnDown, pt1);
								 ScreenToWorld(IPoint2(x, y), pt2);
								 ZoomFitWrold(pt1.x, pt1.y, pt2.x, pt2.y);
	}
		break;
	case GLAYER_STATE_CALCULATING:
		// 		if (m_bLBtnDown)
		// 		{
		// 			DPoint3 pt1,pt2;
		// 			ScreenToWorld(m_ptLBtnDown,pt1);
		// 			ScreenToWorld(IPoint2(x,y),pt2);
		// 			double dis = calculator::distanceLatLon(pt1.x,pt1.y,pt2.x,pt2.y);
		// 			double dir = calculator::directionLatLon(pt1.x,pt1.y,pt2.x,pt2.y);
		// 			dis /= 1000;
		// 			int nDis = int(dis);
		// 
		// 			std::ostringstream strDir,strDis;
		// 			strDir<<dir;
		// 			strDis<<nDis;
		// 			m_strDir = strDir.str() + "度";
		// 			m_strDis = strDis.str() + "公里" + m_strDir;
		// 		}
		break;
	default:
		break;
	}
	m_bLBtnDown = false;
}
void MyGLWidget::OnMouseMove(int x, int y)
{
	switch (m_nState)
	{
	case GLAYER_STATE_RINGING:
		break;
	case GLAYER_STATE_CALCULATING:
		if (m_bLBtnDown)
		{
			DPoint3 pt1, pt2;
			ScreenToWorld(m_ptLBtnDown, pt1);
			ScreenToWorld(IPoint2(x, y), pt2);
			double dis = calculator::distanceLatLon(pt1.x, pt1.y, pt2.x, pt2.y);
			double dir = calculator::directionLatLon(pt1.x, pt1.y, pt2.x, pt2.y);
			dis /= 1000;
			int nDis = int(dis);

			std::ostringstream strDir, strDis;
			strDir << dir;
			strDis << nDis;
			m_strDisDir = strDis.str() + "公里/" + strDir.str() + "度";
		}
		break;
	default:
	{
			   if (m_bLBtnDown)
			   {
				   PanScreenDelta(x - m_ptMouseCurrent.x, m_ptMouseCurrent.y - y);
			   }
			   else if (m_bRBtnDown)
			   {
				   ScreenRoll(y - m_ptMouseCurrent.y, x - m_ptMouseCurrent.x);
			   }
			   else if (m_bMBtnDown)
			   {
				   ScreenRotate(m_ptMouseCurrent.x, m_ptMouseCurrent.y, x, y);
			   }
	}
		break;
	}
	m_ptMouseCurrent.x = x;
	m_ptMouseCurrent.y = y;

}
void MyGLWidget::OnMouseWheel(short zDelta)
{
	ZoomDelta(zDelta> 0 ? 0.1 : -0.1);
}
void MyGLWidget::OnSize(int cx, int cy)
{
	OnSetSize(cx, cy);
}
void MyGLWidget::OnLButtonDblClk(int x, int y)
{

}
void MyGLWidget::OnRButtonDown(int x, int y)
{
	m_bRBtnDown = TRUE;
	m_ptLBtnDown.x = x;
	m_ptLBtnDown.y = y;
	m_ptMouseCurrent.x = x;
	m_ptMouseCurrent.y = y;
}
void MyGLWidget::OnRButtonUp(int x, int y)
{
	m_bRBtnDown = false;
}
void MyGLWidget::OnMButtonDown(int x, int y)
{
	m_bMBtnDown = TRUE;
	m_ptLBtnDown.x = x;
	m_ptLBtnDown.y = y;
	m_ptMouseCurrent.x = x;
	m_ptMouseCurrent.y = y;
}
void MyGLWidget::OnMButtonUp(int x, int y)
{
	m_bMBtnDown = false;
}
//////////////////////////////////////////////////////////////////////////
void MyGLWidget::ScreenToWorld(int sx, int sy, double& wx, double& wy)
{
	DPoint3 wp;
	ScreenToWorld(IPoint2(sx, sy), wp);
	wx = wp.x;
	wy = wp.y;
}
//////////////////////////////////////////////////////////////////////////
void MyGLWidget::OnZoomOut()
{
	ZoomDelta(0.1);
}

void MyGLWidget::OnZoomIn()
{
	ZoomDelta(-0.1);
}

void MyGLWidget::OnZoomBase()
{
	onBase();
}

void MyGLWidget::SetOperation(std::string strOperation)
{
	if (strOperation == "平移")
	{
		m_nState = GLAYER_STATE_MOVING;
	}
	else if (strOperation == "框选")
	{
		m_nState = GLAYER_STATE_RINGING;
	}
	else if (strOperation == "测量")
	{
		m_nState = GLAYER_STATE_CALCULATING;
	}
	else
	{
		m_nState = GLAYER_STATE_MOVING;
	}
}

void MyGLWidget::onBase()
{
	// 	ZoomFitWrold(118.8,29.2,119.2,28.8);
	ZoomFitWrold(-0.4, 0.4, 0.4, -0.4);
}

void MyGLWidget::OnKey(UINT nChar)
{
	/*
	37：左
	38：上
	39：右
	40：下
	*/

}



// bool sortByX(Point& p1,Point &p2)  
// {  
// 	return p1.x<=p2.x ;
// }
// bool sortByY(Point& p1,Point &p2)  
// {  
// 	return p1.y<=p2.y ;
// }
// 
// double calcShortestDistance(std::vector<Point> &pts,int &index1,int &index2)
// {
// 	double dis=100000;	
// 	int size=pts.size();
// 	// 1.小于10个点直接两重循环计算
// 	if (size<10)
// 	{
// 		for (int i=0;i<size;i++)
// 		{
// 			for (int j=0;j<size;j++)
// 			{
// 				if (i!=j)
// 				{
// 					double newDis=calcDis(pts[i],pts[j]);
// 					if ( newDis<dis)
// 					{
// 						index1=pts[i]._ulPos;
// 						index2=pts[j]._ulPos;
// 						dis=newDis;
// 					}
// 				}
// 			}
// 		}
// 		return dis;
// 	}
// 	// 2.分治
// 	vector<Point> pts1;
// 	vector<Point> pts2;
// 
// 	int mid=size/2;
// 	for (int i=0;i<mid;i++)
// 	{
// 		pts1.push_back(pts[i]);
// 	}
// 	for (int i=mid;i<size;i++)
// 	{
// 		pts2.push_back(pts[i]);
// 	}
// 	int id11,id12,id21,id22;
// 	double dis1=calcShortestDistance(pts1,id11,id12);
// 	double dis2=calcShortestDistance(pts2,id21,id22);
// 	if (dis1<dis2)
// 	{
// 		dis=dis1;
// 		index1=id11;
// 		index2=id12;
// 	}
// 	else
// 	{
// 		dis=dis2;
// 		index1=id21;
// 		index2=id22;
// 	}
// 	double dbLeft=pts[mid].x-dis;
// 	double dbRight=pts[mid].x+dis;
// 	int l=mid-1;
// 	int r=mid+1;
// 	while (l>0 && pts[l].x>dbLeft  ) l--;
// 	while (r<size && pts[r].x<dbRight  ) r++;
// 
// 	pts1.clear();
// 	for (int i=l+1;i<r;i++)
// 	{
// 		pts1.push_back(pts[i]);
// 	}
// 	std::sort(pts1.begin(),pts1.end(),sortByY);
// 	size =pts1.size();
// 	for (int i=0;i<size-7;i++)
// 	{
// 		double newDis=calcDis(pts1[i],pts1[i+7]);
// 		if (newDis<dis)
// 		{
// 			dis=newDis;
// 			index1=pts1[i]._ulPos;
// 			index2=pts1[i+7]._ulPos;			
// 		}
// 	}
// 	return dis;
// }
// 
// void GLayer::CalcNearestPairOfPoints_DC(){
// 	
// 	// 1.为每个点设置序号,并创建一个副本:n
// 	vector<Point> pts;
// 	for (int i=0;i<_uiNumberOfPoints;i++)
// 	{
// 		_points[i]._ulPos=i;
// 		pts.push_back(_points[i]);
// 	}
// 	// 2.按照横坐标排序:nlogn
// 	std::sort(pts.begin(),pts.end(),sortByX);
// 	// 3.寻找最近点:nlogn
// 	calcShortestDistance(pts,_uiPointId1,_uiPointId2);
// 	_bCalculated=true;
// }

// 计算两点之间距离
double calcDis(DPoint3 pt1, DPoint3 pt2){
	double xx = pt1.x - pt2.x;
	double yy = pt1.y - pt2.y;
	return sqrt(xx*xx + yy*yy);
}

// 按照Y坐标快排
void QsortY(Point* pts, int low, int high)
{
	if (low >= high)
	{
		return;
	}
	int first = low;
	int last = high;
	Point key = pts[first];/*用字表的第一个记录作为枢轴*/
	while (first<last)
	{
		while (first<last&&pts[last].y >= key.y)
			--last;
		pts[first] = pts[last];/*将比第一个小的移到低端*/
		while (first<last&&pts[first].y <= key.y)
			++first;
		pts[last] = pts[first];/*将比第一个大的移到高端*/
	}
	pts[first] = key;/*枢轴记录到位*/
	QsortY(pts, low, first - 1);
	QsortY(pts, last + 1, high);
}

// 按照X坐标快排
void QsortX(Point* pts, int low, int high)
{
	if (low >= high)
	{
		return;
	}
	int first = low;
	int last = high;
	Point key = pts[first];/*用字表的第一个记录作为枢轴*/
	while (first<last)
	{
		while (first<last&&pts[last].x >= key.x)
			--last;
		pts[first] = pts[last];/*将比第一个小的移到低端*/
		while (first<last&&pts[first].x <= key.x)
			++first;
		pts[last] = pts[first];/*将比第一个大的移到高端*/
	}
	pts[first] = key;/*枢轴记录到位*/
	QsortX(pts, low, first - 1);
	QsortX(pts, last + 1, high);
}

// 计算ptsX中下标从low到high之间最短距离的点对
// 返回其距离，index1，index2分别返回两个点的索引
// ptsY为辅助空间
double calcShortestDistance(const Point* ptsX, Point* ptsY, int low, int high, int &index1, int &index2)
{
	double dis = 100000;
	int size = high - low + 1;
	// 1.小于10个点直接两重循环计算
	if (size<10)
	{
		for (int i = low; i <= high; i++)
		{
			for (int j = low; j <= high; j++)
			{
				if (i != j)
				{
					double newDis = calcDis(ptsX[i], ptsX[j]);
					if (newDis<dis)
					{
						index1 = ptsX[i]._ulPos;
						index2 = ptsX[j]._ulPos;
						dis = newDis;
					}
				}
			}
		}
		return dis;
	}
	// 2.分治
	int mid = low + size / 2;
	int id11, id12, id21, id22;
	// 2.1.按照X坐标分成两半分别计算
	double dis1 = calcShortestDistance(ptsX, ptsY, low, mid, id11, id12);
	double dis2 = calcShortestDistance(ptsX, ptsY, mid + 1, high, id21, id22);
	if (dis1<dis2)
	{
		dis = dis1;
		index1 = id11;
		index2 = id12;
	}
	else
	{
		dis = dis2;
		index1 = id21;
		index2 = id22;
	}
	// 2.2.计算中间带
	double dbLeft = ptsX[mid].x - dis;
	double dbRight = ptsX[mid].x + dis;
	int l = mid - 1;
	int r = mid + 1;
	while (l>0 && ptsX[l].x>dbLeft) l--;
	while (r<size && ptsX[r].x<dbRight) r++;
	for (int i = l + 1; i<r; i++)
	{
		ptsY[i] = ptsX[i];
	}
	QsortY(ptsY, l + 1, r - 1);

	for (int i = l + 1; i<r - 7; i++)
	{
		double newDis = calcDis(ptsY[i], ptsY[i + 7]);
		if (newDis<dis)
		{
			dis = newDis;
			index1 = ptsY[i]._ulPos;
			index2 = ptsY[i + 7]._ulPos;
		}
	}
	return dis;
}

// 计算最近点对
DistanceAndIndices MyGLWidget::CalcNearestPairOfPoints(){
	double dis = 1000;
	for (int i = 0; i<_uiNumberOfPoints; i++)
	{
		for (int j = 0; j<_uiNumberOfPoints; j++)
		{
			if (i != j)
			{
				double newDis = calcDis(_points[i], _points[j]);
				if (newDis<dis)
				{
					dis = newDis;
					_result._nIndex1 = i;
					_result._nIndex2 = j;
				}
			}
		}
	}
	_bCalculated = true;
	_result._dbDis = dis;
	return _result;
}

// 分治计算最近点对
DistanceAndIndices MyGLWidget::CalcNearestPairOfPoints_DC(){
	// 1.将点列置入临时缓冲区，并设置序号(n)
	Point* pTempX = new Point[_uiNumberOfPoints];
	Point* pTempY = new Point[_uiNumberOfPoints];
	for (int i = 0; i<_uiNumberOfPoints; i++)
	{
		pTempX[i] = _points[i];
		pTempX[i]._ulPos = i;
	}
	// 2.按照横坐标排序:nlogn
	QsortX(pTempX, 0, _uiNumberOfPoints - 1);
	// 3.寻找最近点:nlogn
	_result._dbDis = calcShortestDistance(pTempX, pTempY, 0, _uiNumberOfPoints - 1, _result._nIndex1, _result._nIndex2);
	delete[]pTempX;
	delete[]pTempY;
	_bCalculated = true;
	return _result;
}

// 生成一个0~1随机数
double MyRandom(){
	return (double)((RAND_MAX + 1)*(long)rand() + rand()) / RAND_MAX / RAND_MAX;
}

// 生成number个随机点
void MyGLWidget::GenerateRandomPoints(int number){
	_bCalculated = false;
	_uiNumberOfPoints = number;
	_points.clear();
	double dbSpatialScope =/*(number/10000.0)**/4.0;
	for (int i = 0; i<_uiNumberOfPoints; i++)
	{
		_points.push_back(DPoint3(-2 + MyRandom()*4.0, -2 + MyRandom()*4.0, 0));
	}
}

void MyGLWidget::GenerateNormalPoints(int number, double mx, double my, double vx, double vy) {
	if (_pLayer) delete _pLayer;
	_pLayer = ILayer::CreateLayer(ILayer::LT_Normal_Single
		, number, mx, my, vx, vy);
}

void MyGLWidget::GenerateMVNPoints(int number, double mx, double my, double vx, double vy) {
	if (_pLayer) delete _pLayer;
	_pLayer = ILayer::CreateLayer(ILayer::LT_Normal_Multi
		, number, mx, my, vx, vy);
}

// generate a sequene
void MyGLWidget::GenerateSequence() {
	if (_pLayer) delete _pLayer;
	_pLayer = ILayer::CreateLayer(ILayer::LT_Sequence_1D);
}

// generate a sequene
void MyGLWidget::GenerateSequence2D() {
	if (_pLayer) delete _pLayer;
	_pLayer = ILayer::CreateLayer(ILayer::LT_Sequence_2D);
}

// 开启\关闭手动选点
void MyGLWidget::SetHandPoint()
{
	_bHandPoint = !_bHandPoint;
	if (_bHandPoint)
	{
		_points.clear();
		_uiNumberOfPoints = 0;
		_result._nIndex1 = _result._nIndex2 = -1;
	}
}

//////////////////////////////////////////////////////////////////////////OGL
#define POLYGON_STYLE_FILL		1
#define WCHAR_SIZE					100
#define MIN(x,y)							(x)>(y)?(y):(x)
#define MAX(x,y)							(x)>(y)?(x):(y)
//获取两个点的相对角度
//范围：-90~270，X轴正方向为0
double TwoPointToAngel(DPoint3 pt1, DPoint3 pt2)
{
	double xx = pt2.x - pt1.x;
	double yy = pt2.y - pt1.y;
	if (xx == 0)
	{
		return yy > 0 ? 90 : -90;
	}
	double angel = atan(yy / xx) * 180 / 3.14;
	if (xx < 0)
	{
		angel += 180;
	}
	return angel;
}
//范围：-90~270，X轴正方向为0，屏幕坐标，y轴向下
double TwoPointToAngel(int x1, int y1, int x2, int y2)
{
	double xx = x2 - x1;
	double yy = y2 - y1;
	if (xx == 0)
	{
		return yy > 0 ? 90 : -90;
	}
	double angel = atan(yy / xx) * 180 / 3.14;
	if (xx < 0)
	{
		angel += 180;
	}
	return angel;
}

void MyGLWidget::OnCreate()
{
	// Default mode
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	glShadeModel(GL_FLAT);
	glEnable(GL_NORMALIZE);
	// 	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//::glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// 	::glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//这一句让文字有阴影
	//	glEnable( GL_TEXTURE_2D);//这一句让空间的滚动条不可用
	// 	glEnable(GL_DEPTH_TEST);
	//glEnable( GL_LINE_SMOOTH);
	//glEnable(GL_BLEND);
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE); // GL_ONE_MINUS_SRC_ALPHA
	glClearDepth(1.0);
	//polygon tess
	m_pGlTessObj = gluNewTess();
	gluTessCallback(m_pGlTessObj, GLU_TESS_BEGIN, (void (CALLBACK *) ())glBegin);
	gluTessCallback(m_pGlTessObj, GLU_TESS_VERTEX, (void (CALLBACK *) ()) &glVertex3dv);
	gluTessCallback(m_pGlTessObj, GLU_TESS_END, (void (CALLBACK *) ())glEnd);
	gluTessCallback(m_pGlTessObj, GLU_TESS_ERROR, (void (CALLBACK *) ())errorCallback);
	gluTessCallback(m_pGlTessObj, GLU_TESS_COMBINE, (void (CALLBACK *) ())combineCallback);
	gluTessProperty(m_pGlTessObj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
}//----------------------------------------------------------------------------------------------------
void MyGLWidget::OnDestroy()
{
	if (_pLayer)
	{
		delete _pLayer;
	}
	gluDeleteTess(m_pGlTessObj);
}//----------------------------------------------------------------------------------------------------
void MyGLWidget::OnPreRenderScene()
{
	//1.viewport setting
	glViewport(0, 0, m_nClientW, m_nClientH);
	//2.projection mode setting
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(m_dbFovy, (m_nClientH == 0) ? 1.0 : (double)m_nClientW / (double)m_nClientH, m_dbZNear, m_dbZFar);

	//3.clear background
	glDrawBuffer(GL_BACK);
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//4.Model View Setting
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//4.1 View
	gluLookAt(m_pt3Eye.x, m_pt3Eye.y, m_pt3Eye.z, m_pt3Eye.x, m_pt3Eye.y, 0, 0, 1, 0);
}//----------------------------------------------------------------------------------------------------
void MyGLWidget::OnPostRenderScene()
{
	// 	drawEagleView();
}//----------------------------------------------------------------------------------------------------
void MyGLWidget::Transform()
{
	//4.2 Model
	glTranslated(m_pt3Eye.x, m_pt3Eye.y, 0);
	glRotated(m_dbRotat.x, 1.0, 0.0, 0.0);
	glRotated(m_dbRotat.y, 0.0, 1.0, 0.0);
	glRotated(m_dbRotat.z, 0.0, 0.0, 1.0);
	glTranslated(-m_pt3Eye.x, -m_pt3Eye.y, 0);
	//旋转和缩放首先移动到Eye点为中心，这样就可以以视点为基准点。
	//之后移动回原来的位置。
	//平移变换暂时没有使用，平移效果是使用视点来实现的。
	//20120127
	//去掉了状态的保存与恢复，以及GL_DEPTH_TEST的修改
	//20120205
	//模型变换现在只涉及一个旋转问题，平移通过视点和参考点决定。
	//缩放通过视角决定。
	//20120206

	//将变换放在单独的函数中，使外部可以控制变换的作用域。
	//20121017
}
void MyGLWidget::ReTransform()
{
	glTranslated(m_pt3Eye.x, m_pt3Eye.y, 0);
	glRotated(-m_dbRotat.z, 0.0, 0.0, 1.0);
	glRotated(-m_dbRotat.y, 0.0, 1.0, 0.0);
	glRotated(-m_dbRotat.x, 1.0, 0.0, 0.0);
	glTranslated(-m_pt3Eye.x, -m_pt3Eye.y, 0);
}
void MyGLWidget::OnSetSize(int cx, int cy)
{
	m_nClientH = cy;
	m_nClientW = cx;
}//----------------------------------------------------------------------------------------------------
void MyGLWidget::ScreenToWorld(const IPoint2& ps, DPoint3& pw)
{
	// 	//屏幕中心坐标
	// 	int x0=m_nClientW/2;
	// 	int y0=m_nClientH/2;
	// 	//获取绕屏幕中心顺时针旋转m_dbRotate.z后的坐标
	// 	int x1=(ps.x-x0)*cos(m_dbRotat.z)+(y0-ps.y)*sin(m_dbRotat.z)+x0;
	// 	int y1=y0-((y0-ps.y)*cos(m_dbRotat.z)-(ps.x-x0)*sin(m_dbRotat.z));
	// 
	// 	pw.x=m_pt3Eye.x+(x1-m_nClientW/2.0)*GetSWScale();
	// 	pw.y=m_pt3Eye.y+(m_nClientH/2.0-y1)*GetSWScale();

	//不考虑旋转的代码20120201
	pw.x = m_pt3Eye.x + (ps.x - m_nClientW / 2.0)*GetSWScale();
	pw.y = m_pt3Eye.y + (m_nClientH / 2.0 - ps.y)*GetSWScale();

}//----------------------------------------------------------------------------------------------------
void MyGLWidget::WorldToScreen(DPoint3& wp, IPoint2& sp)
{
	//notice, add 0.5
	sp.x = (int)((wp.x - m_pt3Eye.x) / GetSWScale() + m_nClientW / 2.0 + 0.5);
	sp.y = (int)(m_nClientH / 2.0 - (wp.y - m_pt3Eye.y) / GetSWScale() + 0.5);
}//----------------------------------------------------------------------------------------------------
void MyGLWidget::PanScreenDelta(int cx, int cy)
{
	double scale = GetSWScale();
	m_pt3Eye.x -= cx*scale;//(x*cos(m_rotation.z)-y*sin(m_rotation.z));
	m_pt3Eye.y -= cy*scale;//(y*cos(m_rotation.z)+x*sin(m_rotation.z));
	//cx,cy表示显示器上平移的距离。
	//需要除以缩放比例，让后获取试图对应的距离，然后作用在视点（焦点）上。
}//----------------------------------------------------------------------------------------------------
//20111208注释，暂不考虑屏幕旋转
//20120127重新启用，决定再次加入旋转
void MyGLWidget::ScreenRotate(int x1, int y1, int x2, int y2)
{
	// 	DPoint3 pt1;
	// 	DPoint3 pt2;
	// 	ScreenToWorld(IPoint2(x1,y1),pt1);
	// 	ScreenToWorld(IPoint2(x2,y2),pt2);
	// 	double angel1=TwoPointToAngel(m_pt3Eye,pt1);
	// 	double angel2=TwoPointToAngel(m_pt3Eye,pt2);
	// 	double angel = m_dbRotat.z+angel2-angel1;
	// 	if (angel>360)
	// 	{
	// 		angel-=360;
	// 	}
	// 	else if (angel<0)
	// 	{
	// 		angel+=360;
	// 	}
	// 	m_dbRotat.z=angel;
	//以上代码在世界坐标系内进行旋转，现在改为在屏幕坐标系内旋转，20120201


	//屏幕中心坐标
	int x0 = m_nClientW / 2;
	int y0 = m_nClientH / 2;

	//求旋转角
	double angle1 = TwoPointToAngel(x0, y0, x1, y1);
	double angle2 = TwoPointToAngel(x0, y0, x2, y2);

	double angel = m_dbRotat.z + angle1 - angle2;
	if (angel > 360)
	{
		angel -= 360;
	}
	else if (angel < 0)
	{
		angel += 360;
	}
	m_dbRotat.z = angel;
}//----------------------------------------------------------------------------------------------------
//20121011沿x,y轴旋转屏幕。
void MyGLWidget::ScreenRoll(int cx, int cy)
{
	m_dbRotat.x += cx / 4.0;
	m_dbRotat.y += cy / 4.0;
}
void MyGLWidget::ZoomDelta(double dScale)
{
	m_dbFovy *= (1 - dScale / 2.0);
	m_dbFovy > 180 ? m_dbFovy = 180 : 0;
	//如果视野大于180度，就会出现镜面效果，而这是应该避免的。20121011
	// 	m_dbScale*=(1+dScale);
	//之前使用z坐标调整比率的代码
	// 	double z=m_dbEyeZ-m_dbEyeZ*dScale;
	// 	if(z<m_dbZFar && z>m_dbZNear)
	// 	{
	// 		m_dbEyeZ=z;
	// 		calculateSWScale();
	// 	}
}//----------------------------------------------------------------------------------------------------
//20120105
void MyGLWidget::ZoomFitWrold(double e, double n, double w, double s)
{

	char c[50];
	sprintf(c, "ClientW:%d\nClientH:%d\n", m_nClientW, m_nClientH);
	//Reset Eye Point
	m_pt3Eye = DPoint3((e + w) / 2, (n + s) / 2, m_pt3Eye.z);

	//Get the Key factor(width or height which present a larger area)
	double dbWidth = abs(e - w);
	double dbHeight = abs(n - s);
	double dbHeightT = dbWidth*(double)m_nClientH / (double)m_nClientW;
	double dbKey = MAX(dbHeight, dbHeightT);
	//adjust m_dbScale
	// 	m_dbScale=m_pt3Eye.z*(tan(m_dbFovy*PId/180.0/2)*2.0)/dbKey;
	m_dbFovy = atan(dbKey / m_pt3Eye.z / 2.0)*180.0*2.0 / PId;

	//Reset Three Transform Factor
	m_dbRotat = DPoint3(0, 0, 0);
	// 	m_dbTranslate=DPoint3(0,0,0);
	//m_dbScale=1;
}//----------------------------------------------------------------------------------------------------

void CALLBACK MyGLWidget::errorCallback(GLenum errorCode)
{
	const GLubyte *estring;

	estring = gluErrorString(errorCode);
	fprintf(stderr, "Tessellation Error: %s\n", estring);
	exit(0);
}//----------------------------------------------------------------------------------------------------
void CALLBACK MyGLWidget::combineCallback(GLdouble coords[3], GLdouble *data[4], GLfloat weight[4], GLdouble **dataOut)
{
	GLdouble *vertex;
	vertex = (GLdouble *)malloc(3 * sizeof(GLdouble));

	vertex[0] = coords[0];
	vertex[1] = coords[1];
	vertex[2] = coords[2];
	*dataOut = vertex;
}//----------------------------------------------------------------------------------------------------


void MyGLWidget::SetLineWidth(const float wt)
{
	glLineWidth(wt);
}//----------------------------------------------------------------------------------------------------
void MyGLWidget::SetPointSize(const float ps)
{
	glPointSize(ps);
}//----------------------------------------------------------------------------------------------------
void MyGLWidget::SetColor(const RGBAf& col)
{
	// 	m_currentColor=col;
	glColor4f(col.r, col.g, col.b, col.a);
}//----------------------------------------------------------------------------------------------------
void MyGLWidget::EnableBlend(bool b)
{
	if (b)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE); // GL_ONE_MINUS_SRC_ALPHA
		return;
	}
	glDisable(GL_BLEND);
}//----------------------------------------------------------------------------------------------------
void MyGLWidget::SetClearColor(float r, float g, float b, float a)
{
	m_clearColor.r = r;
	m_clearColor.g = g;
	m_clearColor.b = b;
	m_clearColor.a = a;
}
void MyGLWidget::DrawImage(DPoint3& ptWS, DPoint3& ptEN, GLsizei imgWidth, GLsizei imgHeight, unsigned char* pData)
{
	GLuint texture = 0;
	EnableBlend(true);
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBegin(GL_POLYGON);
	//	glColor4f(0.5,0.5,0.5,1.0);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(ptEN.x, ptEN.y);

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(ptWS.x, ptEN.y);

	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(ptWS.x, ptWS.y);

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(ptEN.x, ptWS.y);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDeleteTextures(1, &texture);

	EnableBlend(false);
}
void MyGLWidget::SetPolygonStyle(short style)
{
	m_sPolyStyle = style;
}

void MyGLWidget::DrawPoint(const DPoint3& p)
{
	glBegin(GL_POINTS);
	glVertex3d(p.x, p.y, p.z);
	glEnd();
}//----------------------------------------------------------------------------------------------------
void MyGLWidget::DrawLine(const DPoint3& p1, const DPoint3& p2)
{
	glBegin(GL_LINE_STRIP);
	glVertex3d(p1.x, p1.y, 0);
	glVertex3d(p2.x, p2.y, 0);
	glEnd();
}//----------------------------------------------------------------------------------------------------
void MyGLWidget::DrawLine3D(const DPoint3& p1, const DPoint3& p2)
{
	glBegin(GL_LINE_STRIP);
	glVertex3d(p1.x, p1.y, p1.z);
	glVertex3d(p2.x, p2.y, p2.z);
	glEnd();
}//----------------------------------------------------------------------------------------------------

void MyGLWidget::DrawLine(const std::vector<DPoint3> &dline, bool bClose)
{
	size_t size = dline.size();
	if (size < 2) return;
	bClose ? glBegin(GL_LINE_LOOP) : glBegin(GL_LINE_STRIP);
	for (size_t i = 0; i < size; i++)
	{
		glVertex3d(dline[i].x, dline[i].y, 0);
	}
	glEnd();
}//----------------------------------------------------------------------------------------------------
void MyGLWidget::DrawLine3D(const std::vector<DPoint3> &dline, bool bClose)
{
	size_t size = dline.size();
	if (size < 2) return;
	bClose ? glBegin(GL_LINE_LOOP) : glBegin(GL_LINE_STRIP);
	for (size_t i = 0; i < size; i++)
	{
		glVertex3d(dline[i].x, dline[i].y, dline[i].z);
	}
	glEnd();
}//----------------------------------------------------------------------------------------------------

void MyGLWidget::DrawLine2D(const IPoint2& pt1, const IPoint2& pt2)
{
	DPoint3 p1, p2;
	ScreenToWorld(pt1, p1);
	ScreenToWorld(pt2, p2);
	glBegin(GL_LINE_STRIP);
	glVertex3d(p1.x, p1.y, 0);
	glVertex3d(p2.x, p2.y, 0);
	glEnd();
}

void MyGLWidget::DrawRectangle(const DPoint3& p1, const DPoint3& p2)
{
	glBegin(GL_LINE_LOOP);
	glVertex3d(p1.x, p1.y, 0);
	glVertex3d(p1.x, p2.y, 0);
	glVertex3d(p2.x, p2.y, 0);
	glVertex3d(p2.x, p1.y, 0);
	glEnd();
}//----------------------------------------------------------------------------------------------------
void MyGLWidget::DrawRectangle(const IPoint2& pt1, const IPoint2& pt2)
{
	DPoint3 p1, p2;
	ScreenToWorld(pt1, p1);
	ScreenToWorld(pt2, p2);
	glBegin(GL_LINE_LOOP);
	glVertex3d(p1.x, p1.y, 0);
	glVertex3d(p1.x, p2.y, 0);
	glVertex3d(p2.x, p2.y, 0);
	glVertex3d(p2.x, p1.y, 0);
	glEnd();
}//----------------------------------------------------------------------------------------------------
void MyGLWidget::DrawRectangleO(const IPoint2& pt1, const IPoint2& pt2)
{
	DPoint3 p1, p2, p3, p4, p5, p6, p7, p8;
	ScreenToWorld(pt1 + IPoint2(8, 0), p1);
	ScreenToWorld(pt1 + IPoint2(4, 2), p2);
	ScreenToWorld(pt1 + IPoint2(2, 4), p3);
	ScreenToWorld(pt1 + IPoint2(0, 8), p4);

	ScreenToWorld(pt2 + IPoint2(-8, 0), p5);
	ScreenToWorld(pt2 + IPoint2(-4, -2), p6);
	ScreenToWorld(pt2 + IPoint2(-2, -4), p7);
	ScreenToWorld(pt2 + IPoint2(0, -8), p8);

	glBegin(GL_LINE_LOOP);
	glVertex3d(p1.x, p1.y, 0);
	glVertex3d(p2.x, p2.y, 0);
	glVertex3d(p3.x, p3.y, 0);
	glVertex3d(p4.x, p4.y, 0);
	glVertex3d(p4.x, p8.y, 0);
	glVertex3d(p3.x, p7.y, 0);
	glVertex3d(p2.x, p6.y, 0);
	glVertex3d(p1.x, p5.y, 0);
	glVertex3d(p5.x, p5.y, 0);
	glVertex3d(p6.x, p6.y, 0);
	glVertex3d(p7.x, p7.y, 0);
	glVertex3d(p8.x, p8.y, 0);

	glVertex3d(p8.x, p4.y, 0);
	glVertex3d(p7.x, p3.y, 0);
	glVertex3d(p6.x, p2.y, 0);
	glVertex3d(p5.x, p1.y, 0);
	glEnd();

}//----------------------------------------------------------------------------------------------------

void MyGLWidget::DrawPolygon(const std::vector<std::vector<DPoint3>> &poly)
{
	//size of contours
	size_t contours = poly.size();
	if (m_sPolyStyle == POLYGON_STYLE_FILL)
	{
		size_t p = 0;
		static GLdouble v[32000][3];
		//only one polygon, but multiple contours
		gluTessBeginPolygon(m_pGlTessObj, NULL);
		for (size_t i = 0; i < contours; i++)
		{
			size_t size = poly[i].size();
			gluTessBeginContour(m_pGlTessObj);

			for (size_t k = 0; k < size; k++)
			{
				v[p][0] = poly[i][k].x;
				v[p][1] = poly[i][k].y;
				v[p][2] = poly[i][k].z;
				gluTessVertex(m_pGlTessObj, v[p], v[p]);
				p++;
			}
			gluTessEndContour(m_pGlTessObj);
		}
		gluTessEndPolygon(m_pGlTessObj);
		//glPopAttrib();
	}
	else
	{
		// just draw each ring
		for (int i = 0; i < contours; i++) DrawLine(poly[i], true);
	}
}//----------------------------------------------------------------------------------------------------
void MyGLWidget::DrawPolygons(const std::vector<std::vector<std::vector<DPoint3>>> &polys)
{
	//size of contours
	size_t nPolys = polys.size();
	if (m_sPolyStyle == POLYGON_STYLE_FILL)
	{
		size_t p = 0;
		static GLdouble v[32000][3];
		for (int i = 0; i < nPolys; i++)
		{
			size_t contours = polys[i].size();
			//only one polygon, but multiple contours
			gluTessBeginPolygon(m_pGlTessObj, NULL);
			for (size_t j = 0; j < contours; j++)
			{
				size_t size = polys[i][j].size();
				gluTessBeginContour(m_pGlTessObj);

				for (size_t k = 0; k < size; k++)
				{
					v[p][0] = polys[i][j][k].x;
					v[p][1] = polys[i][j][k].y;
					v[p][2] = 0.0;
					gluTessVertex(m_pGlTessObj, v[p], v[p]);
					p++;
				}
				gluTessEndContour(m_pGlTessObj);
			}
			gluTessEndPolygon(m_pGlTessObj);
		}
	}
	else
	{
		// just draw each ring
		for (int i = 0; i < nPolys; i++)
		{
			size_t contours = polys[i].size();
			for (int j = 0; j < contours; j++)
			{
				DrawLine(polys[i][j], true);
			}
		}
	}
}//----------------------------------------------------------------------------------------------------
//20121017
void MyGLWidget::DrawCircle(DPoint3 center, double radius)
{
	std::vector<DPoint3> circle;
	double pi = 3.14;
	for (int i = 0; i < 360; i++)
	{
		DPoint3 pt;
		pt.x = center.x + radius*sin((double)i*3.14 / 180);
		pt.y = center.y + radius*cos((double)i*3.14 / 180);
		circle.push_back(pt);
	}
	DrawLine(circle, true);
}
//20111208
void MyGLWidget::drawEagleView()
{
	glReadBuffer(GL_BACK);
	IPoint2 ptS((double)m_nClientW*0.8, (double)m_nClientH*0.9);
	DPoint3 ptW;
	ScreenToWorld(ptS, ptW);
	glRasterPos3d(ptW.x, ptW.y, 0);
	glPixelZoom(0.1f, 0.1f);
	glCopyPixels(0, 0, m_nClientW, m_nClientH, GL_COLOR);
	glPixelZoom(1, 1);
}
void MyGLWidget::DrawRect2D(const IPoint2& pt1, const IPoint2& pt2)
{
	DPoint3 p1, p2;
	ScreenToWorld(pt1, p1);
	ScreenToWorld(pt2, p2);
	glRectf(p1.x, p1.y, p2.x, p2.y);
}
void MyGLWidget::DrawPoint2D(const IPoint2& p)
{
	DPoint3 pt;
	ScreenToWorld(p, pt);
	DrawPoint(pt);
}
void MyGLWidget::DrawRect3D(const DPoint3& pt1, const DPoint3& pt2)
{
	glRectf(pt1.x, pt1.y, pt2.x, pt2.y);
}

void MyGLWidget::onShowBackground(bool bChecked) {
	if (_pLayer)
		_pLayer->ShowBackground(bChecked);
	updateGL();
}
