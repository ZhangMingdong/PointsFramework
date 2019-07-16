#include "NormalPointsLayer.h"

#include<QDebug>



NormalPointsLayer::NormalPointsLayer()
{
}


NormalPointsLayer::~NormalPointsLayer()
{
	if (_arrEdge) delete[]_arrEdge;
}

void NormalPointsLayer::Draw() {

}

const int g_nNeighbors = 4;

void NormalPointsLayer::buildEdgeMatrix() {
	if (_arrEdge) delete[]_arrEdge;
	int nPoints = _points.size();
	_arrEdge = new bool[nPoints*nPoints];
	for (size_t i = 0; i < nPoints*nPoints; i++) _arrEdge[i] = false;
	


	for (size_t i = 0; i < nPoints; i++)
	{
		int arrIndex[g_nNeighbors];
		double arrDistance[g_nNeighbors];
		for (size_t i = 0; i < g_nNeighbors; i++) arrDistance[i] = 1000000;
		// find the nearest g_nNeighbors neighbors of each point
		for (size_t j = 0; j < nPoints; j++)
		{
			if (i == j) continue;
			double dbX = _points[i].x - _points[j].x;
			double dbY = _points[i].y - _points[j].y;
			double dbDistance2 = dbX * dbX + dbY * dbY;
			int k = 0;
			while (k<g_nNeighbors && dbDistance2>arrDistance[k]) k++;
			if (k < g_nNeighbors) {
				for (size_t l = g_nNeighbors-1; l >k; l--)
				{
					arrIndex[l] = arrIndex[l - 1];
					arrDistance[l] = arrDistance[l - 1];
				}
				arrIndex[k] = j;
				arrDistance[k] = dbDistance2;
			}
		}
		// record these edge
		for (size_t j = 0; j < g_nNeighbors; j++)
		{
			if (i> arrIndex[j])
			{
				_arrEdge[arrIndex[j] *nPoints + i] = true;
			}
			else{
				_arrEdge[i*nPoints + arrIndex[j]] = true;
			}

		}

	}
}