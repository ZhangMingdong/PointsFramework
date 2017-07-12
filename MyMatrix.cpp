#include "MyMatrix.h"



MyMatrix::MyMatrix(int r, int c):_nR(r),_nC(c)
{
	allocateSpace();
}

MyMatrix::MyMatrix(const MyMatrix* m1, const MyMatrix* m2)
{
	_nR = m1->_nR;
	_nC = m2->_nC;
	allocateSpace();
	if (m1->_nC==m2->_nR)
	{
		for (size_t i = 0; i < _nR; i++) 
		{
			for (size_t j = 0; j < _nC; j++) 
			{
				for (size_t k = 0; k < m1->_nC; k++) {
					_data[i][j] += m1->GetValue(i, k)*m2->GetValue(k, j);
				}
			}
		}
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
double MyMatrix::GetValue(int i, int j)const {
	return _data[i][j];
}

// set a value
void MyMatrix::SetValue(int i, int j, double v) {
	_data[i][j] = v;
}


void MyMatrix::allocateSpace() {
	_data = new double*[_nR];
	for (size_t i = 0; i < _nR; i++)
	{
		_data[i] = new double[_nC];
	}
}

MyMatrix MyMatrix::Transpose() {
	MyMatrix result(_nC, _nR);
	for (size_t i = 0; i < _nR; i++)
	{
		for (size_t j = 0; j < _nC; j++)
		{
			result.SetValue(j, i, _data[i][j]);
		}
	}
	return result;
}

void MyMatrix::TrimNegative() {
	for (size_t i = 0; i < _nR; i++)
	{
		for (size_t j = 0; j < _nC; j++)
		{
			if (_data[i][j] < 0) _data[i][j] = 0;
		}
	}
}
void MyMatrix::Multi(double dbFactor) {
	for (size_t i = 0; i < _nR; i++)
	{
		for (size_t j = 0; j < _nC; j++)
		{
			_data[i][j] *= dbFactor;
		}
	}
}