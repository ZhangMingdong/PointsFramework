#include "RandomPointsLayer.h"


#include <QGLWidget>
#include <gl/GLU.h>


// ��������֮�����
double calcDis(DPoint3 pt1, DPoint3 pt2) {
	double xx = pt1.x - pt2.x;
	double yy = pt1.y - pt2.y;
	return sqrt(xx*xx + yy*yy);
}

// ����Y�������
void QsortY(DPoint3* pts, int low, int high)
{
	if (low >= high)
	{
		return;
	}
	int first = low;
	int last = high;
	DPoint3 key = pts[first];/*���ֱ�ĵ�һ����¼��Ϊ����*/
	while (first<last)
	{
		while (first<last&&pts[last].y >= key.y)
			--last;
		pts[first] = pts[last];/*���ȵ�һ��С���Ƶ��Ͷ�*/
		while (first<last&&pts[first].y <= key.y)
			++first;
		pts[last] = pts[first];/*���ȵ�һ������Ƶ��߶�*/
	}
	pts[first] = key;/*�����¼��λ*/
	QsortY(pts, low, first - 1);
	QsortY(pts, last + 1, high);
}

// ����X�������
void QsortX(DPoint3* pts, int low, int high)
{
	if (low >= high)
	{
		return;
	}
	int first = low;
	int last = high;
	DPoint3 key = pts[first];/*���ֱ�ĵ�һ����¼��Ϊ����*/
	while (first<last)
	{
		while (first<last&&pts[last].x >= key.x)
			--last;
		pts[first] = pts[last];/*���ȵ�һ��С���Ƶ��Ͷ�*/
		while (first<last&&pts[first].x <= key.x)
			++first;
		pts[last] = pts[first];/*���ȵ�һ������Ƶ��߶�*/
	}
	pts[first] = key;/*�����¼��λ*/
	QsortX(pts, low, first - 1);
	QsortX(pts, last + 1, high);
}

// ����ptsX���±��low��high֮����̾���ĵ��
// ��������룬index1��index2�ֱ𷵻������������
// ptsYΪ�����ռ�
double calcShortestDistance(const DPoint3* ptsX, DPoint3* ptsY, int low, int high, int &index1, int &index2)
{
	double dis = 100000;
	int size = high - low + 1;
	// 1.С��10����ֱ������ѭ������
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
	// 2.����
	int mid = low + size / 2;
	int id11, id12, id21, id22;
	// 2.1.����X����ֳ�����ֱ����
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
	// 2.2.�����м��
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

// ����һ��0~1�����
double RandomPointsLayer::MyRandom() {
	return (double)((RAND_MAX + 1)*(long)rand() + rand()) / RAND_MAX / RAND_MAX;
}

RandomPointsLayer::RandomPointsLayer(int nPoints): _bCalculated(false)
{
	_result._nIndex2 = -1;
	_result._dbDis = 0;

	_bCalculated = false;
	generatePoints(nPoints);

}

void RandomPointsLayer::generatePoints(int nPoints) {
	_points.clear();
	double dbSpatialScope =/*(number/10000.0)**/4.0;
	for (int i = 0; i<nPoints; i++)
	{
		_points.push_back(DPoint3(-dbSpatialScope / 2 + MyRandom()*dbSpatialScope, -dbSpatialScope / 2 + MyRandom()*dbSpatialScope, 0));
	}
}

RandomPointsLayer::~RandomPointsLayer()
{
}

void RandomPointsLayer::Draw() {

	int nLen = _points.size();
	// draw points
	glColor4f(0, 1, 1, 1);
	for (int i = 0; i<nLen; i++)
	{
		glBegin(GL_POINTS);
		glVertex3d(_points[i].x, _points[i].y, _points[i].z);
		glEnd();
	}


	// draw points of class two
	glColor4f(1, 1, 0, 1);
	int nLenR = _pointsR.size();
	for (int i = 0; i<nLenR; i++)
	{
		glBegin(GL_POINTS);
		glVertex3d(_pointsR[i].x, _pointsR[i].y, _pointsR[i].z);
		glEnd();
	}

	// draw the result
	if (_bCalculated&&nLen>0)
	{
		glColor4f(1, 0, 0, 1);
		glBegin(GL_LINE_STRIP);
		glVertex3d(_points[_result._nIndex1].x, _points[_result._nIndex1].y, 0);
		glVertex3d(_points[_result._nIndex2].x, _points[_result._nIndex2].y, 0);
		glEnd();
		double dbRadius = _result._dbDis * 2;
		DrawCircle(_points[_result._nIndex1], dbRadius);
		if (dbRadius<0.01)
		{
			DrawCircle(_points[_result._nIndex1], 0.1);
		}
	}
}

DistanceAndIndices RandomPointsLayer::Calculate(bool bDC) {
	int nLen = _points.size();
	if (bDC)
	{
		// 1.������������ʱ�����������������(n)
		DPoint3* pTempX = new DPoint3[nLen];
		DPoint3* pTempY = new DPoint3[nLen];
		for (int i = 0; i<nLen; i++)
		{
			pTempX[i] = _points[i];
			pTempX[i]._ulPos = i;
		}
		// 2.���պ���������:nlogn
		QsortX(pTempX, 0, nLen - 1);
		// 3.Ѱ�������:nlogn
		_result._dbDis = calcShortestDistance(pTempX, pTempY, 0, nLen - 1, _result._nIndex1, _result._nIndex2);
		delete[]pTempX;
		delete[]pTempY;
		_bCalculated = true;
		return _result;
	}
	else {
		double dis = 1000;
		for (int i = 0; i<nLen; i++)
		{
			for (int j = 0; j<nLen; j++)
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
}


void RandomPointsLayer::Clear() {
	_points.clear();
	_result._nIndex1 = _result._nIndex2 = -1;
}


void RandomPointsLayer::AddPoint(DPoint3 pt, bool bRight) {
	if (bRight)
	{
		_pointsR.push_back(pt);
	}
	else {
		_points.push_back(pt);
	}
};