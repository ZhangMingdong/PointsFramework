#pragma once
#include "ILayer.h"
#include "MathTypes.h"
#include <vector>
/*
	Layer to show the 1D sequence
	Mingdong
	2017/06/21
*/
class Sequence1DLayer :
	public ILayer
{
public:
	Sequence1DLayer();
	virtual ~Sequence1DLayer();
public:
	virtual void Draw();
	// reset length of sample points and period of the function
	virtual void Reset(int nLen, int nPeriod);
private:
	// the original sequence
	std::vector<Point> _sequence;
	// interpolation result of the sequence
	std::vector<Point> _sequenceResultRBF;
	std::vector<Point> _sequenceResultLagrangian;


	std::vector<double> a, b;
	// left and right position
	double _dbLeft;
	double _dbRight;
private:
	// do RBF interpolation
	void doRBF();
	// do Lagrangian interpolation
	void doLagrangian();
protected:
	// generate the sequence
	virtual void generateSequence(int nLen=10, int nPeriod=1);
};

