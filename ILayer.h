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
	};
	static ILayer* CreateLayer(EnumLayerType type);
public:
	virtual void Draw()=0;
	void ShowBackground(bool bShow) { _bShowBackground = bShow; }
protected:

	bool _bShowBackground;

};


double funPhi(double r);