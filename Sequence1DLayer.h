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
	std::vector<Point> _sequence;
	// RBF interpolation result of the sequence
	std::vector<Point> _sequenceResultRBF;
	// Lagrangian interpolation result of the sequence
	std::vector<Point> _sequenceResultLagrangian;
	// KDE interpolation result of the sequence
	std::vector<Point> _sequenceResultKDE;


	std::vector<double> a, b;
	// left and right position
	double _dbLeft;
	double _dbRight;

	// parameter h of kde
	double _dbH=1.0;
private:
	// do RBF interpolation
	void doRBF();
	// do Lagrangian interpolation
	void doLagrangian();
	// do kernel density estimation 
	void doKDE();
protected:
	// generate the sequence
	virtual void generateSequence(int nLen=10, int nPeriod=1);

};

