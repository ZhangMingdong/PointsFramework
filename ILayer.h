#pragma once
#include <MathTypes.hpp>



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
		, LT_Sequence_2D
		, LT_Normal_Single
		, LT_Normal_Multi
		, LT_Normal_Blue
		, LT_Random
		, LT_Random_Blue
		, LT_Random_Blue_Mult
		, LT_Dual
		, LT_Spiral
		, LT_DR
		, LT_Data
	};
	static ILayer* CreateLayer(EnumLayerType type,bool bShowBg,int nPoints=0);
public:
	virtual void Draw()=0;
	// add a point
	virtual void AddPoint(Point pt, bool bRight = false) {};
	// calculate the nearest points pair
	virtual DistanceAndIndices Calculate(bool bDC) {
		return DistanceAndIndices();
	}
	// clear the calculation and points for add points by hand
	virtual void Clear() {};
	// initialization
	virtual void Initialize() {};
	// update this layer, used for the display of classes in multiclass blue noise layer
	virtual void UpdateLayer() {};


	// reset length of sample points and period of the function
	virtual void Reset(int nLen, int nPeriod) {};

	// set the interpolation method/ clusterin gmethod
	virtual void SetMethod(int nMethod) {};

	// set the method to generate the source
	virtual void SetSource(int nSource) {};

	virtual void SetRadius(double r) { _dbPhiRadius = r; };


	virtual void SetClusteringMethod(int method) { _nClusteringMethod = method; UpdateLayer();};
	virtual void SetMinPts(int minPts) { _nMinPts = minPts; UpdateLayer();};

	virtual void SetEps(double eps) { _dbEps = eps; UpdateLayer(); };
public:
	void ShowBackground(bool bShow) { _bShowBackground = bShow; }

protected:
	// whether show background
	bool _bShowBackground;

	// radius for phi function
	double _dbPhiRadius=1;

	double _dbEps = 1;
	int _nMinPts = 10;
	int _nClusteringMethod = 0;

protected:// drawing
	void DrawPoint(const DPoint3& p);

	void DrawLine(const DPoint3& p1, const DPoint3& p2);

	void DrawLine(const std::vector<DPoint3> &dline, bool bClose);		//draw a line

	void DrawCircle(DPoint3 center, double radius);

	// return true if the distance between pt and every points in list are all larger than dis
	bool distanceCheck(const Point& pt, const std::vector<Point>& list, double dis);

public:
	// generate a list of points obey the normal distribution
	static void GenerateNormalPoints(std::vector<Point>& vecPts,int number, double mx, double my, double vx, double vy, double dbBiasX = 0, double dbBiasY = 0);


	// generate a list of points obey the random distribution
	static void GenerateRandomPoints(std::vector<Point>& vecPts, int number, double x,double y,double r);

};

// phi function used in RBF with Gaussian kernel
double funPhi(double x,double r);