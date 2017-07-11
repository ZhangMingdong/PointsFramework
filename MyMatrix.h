#pragma once
// maxtrix structure
class MyMatrix
{
public:
	MyMatrix(int r,int c);
	virtual ~MyMatrix();
private:
	int _nR;				// rows
	int _nC;				// columns
	double** _data;			// the data
public:
	// get a value
	double GetValue(int i, int j);
	// set a value
	void SetValue(int i, int j, double v);
};

