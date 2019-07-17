#pragma once
#include "ILayer.h"
#include <vector>

/*
	Layer to show the grid
	Mingdong
	2019/07/17
*/

// length of the grid;
const int g_nLength = 101;

class GridLayer :
	public ILayer
{
public:
	GridLayer(int nPoints);
	virtual ~GridLayer();
public:
	virtual void Draw();
	virtual void Clear();
protected:
	// DPoint3 list
	DPoint3 _gridRegular[g_nLength][g_nLength];
	DPoint3 _gridDistort[g_nLength][g_nLength];


public:
	// mouse event, word point
	virtual void OnMouse(DPoint3 pt);
};

