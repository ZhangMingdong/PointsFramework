#pragma once
#include "ILayer.h"
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
	std::vector<DPoint3> _sequence;
	// RBF interpolation result of the sequence
	std::vector<DPoint3> _sequenceResultRBF;
	// Lagrangian interpolation result of the sequence
	std::vector<DPoint3> _sequenceResultLagrangian;
	// KDE interpolation result of the sequence
	std::vector<DPoint3> _sequenceResultKDE;
	// result of Shepard's interpolation
	std::vector<DPoint3> _sequenceResultShepards;
	// left and right position
	double _dbLeft=-4;
	double _dbRight=4;
	// parameter h of kde
	double _dbH = 1.0;
	// length of the result sequence
	int _nResultLen = 201;
private:
	// do RBF interpolation
	void doRBF();
	/*
		do Lagrangian interpolation
		<<fundamentals of Numerical Analysis>>, P185
	*/
	void doLagrangian();
	// do kernel density estimation 
	void doKDE();
	// do Shepard's interpolation
	// SIGGRAPH course
	void doShepards();
protected:
	// generate the sequence
	virtual void generateSequence(int nLen=10, int nPeriod=1);
	void generateSinCurve();

};

