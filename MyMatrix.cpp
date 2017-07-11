#include "MyMatrix.h"



MyMatrix::MyMatrix(int r, int c):_nR(r),_nC(c)
{
	_data = new double*[_nR];
	for (size_t i = 0; i < _nR; i++)
	{
		_data[i] = new double[_nC];
	}
}


MyMatrix::~MyMatrix()
{
	for (size_t i = 0; i < _nR; i++)
	{
		delete[] _data[i];
	}
	delete[]_data;
}

// get a value
double MyMatrix::GetValue(int i, int j) {
	return _data[i][j];
}
// set a value
void MyMatrix::SetValue(int i, int j, double v) {
	_data[i][j] = v;
}