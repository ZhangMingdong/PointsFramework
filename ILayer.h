#pragma once
#include"MathTypes.h"
/*
	Interface of layer
	Mingdong
	2017/06/21
*/
class ILayer
{
public:
	ILayer();
	virtual ~ILayer();
public:
	enum EnumLayerType
	{
		LT_Sequence_1D
		,LT_Sequence_2D
		,LT_Normal_Single
		,LT_Normal_Multi
		,LT_Random
	};
	static ILayer* CreateLayer(EnumLayerType type,bool bShowBg,int nPoints=0, double mx=0, double my = 0, double vx = 0, double vy = 0);
public:
	virtual void Draw()=0;
	// add a point
	virtual void AddPoint(Point pt) {};
	// calculate the nearest points pair
	virtual DistanceAndIndices Calculate(bool bDC) {
		return DistanceAndIndices();
	}
	// clear the calculation and points for add points by hand
	virtual void Clear() {};
public:
	void ShowBackground(bool bShow) { _bShowBackground = bShow; }

protected:
	// whether show background
	bool _bShowBackground;

protected:// drawing
	void DrawPoint(const DPoint3& p);

	void DrawLine(const DPoint3& p1, const DPoint3& p2);
	void DrawLine(const std::vector<DPoint3> &dline, bool bClose);		//draw a line

	void DrawCircle(DPoint3 center, double radius);

};

// phi function used in RBF with Gaussian kernel
double funPhi(double r);