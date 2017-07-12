#pragma once
// maxtrix structure
class MyMatrix
{
public:
	MyMatrix(int r,int c);
	// construct from multiplying two matrix
	MyMatrix(const MyMatrix* m1, const MyMatrix* m2);
	virtual ~MyMatrix();
private:
	int _nR;				// rows
	int _nC;				// columns
	double** _data;			// the data
public:
	// get a value
	double GetValue(int i, int j)const;
	// set a value
	void SetValue(int i, int j, double v);
	// generate the transpose matrix
	MyMatrix Transpose();
	// trim the negative to 0
	void TrimNegative();
	// multiply a factor
	void Multi(double dbFactor);
private:
	// allocate space for _data
	void allocateSpace();

};

