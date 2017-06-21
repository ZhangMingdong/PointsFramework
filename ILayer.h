#pragma once
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
	};
	static ILayer* CreateLayer(EnumLayerType type,int nPoints=0, double mx=0, double my = 0, double vx = 0, double vy = 0);
public:
	virtual void Draw()=0;
	void ShowBackground(bool bShow) { _bShowBackground = bShow; }
protected:

	bool _bShowBackground;

};

// phi function used in RBF with Gaussian kernel
double funPhi(double r);