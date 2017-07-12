#include "MyMatrix.h"
#include <assert.h>
#include <random>


MyMatrix::MyMatrix(int r, int c):_nR(r),_nC(c)
{
	allocateSpace();
}

MyMatrix::MyMatrix(const MyMatrix* m1, const MyMatrix* m2)
{
	_nR = m1->_nR;
	_nC = m2->_nC;
	allocateSpace();
	Multi(m1, m2);
}

MyMatrix::MyMatrix(const MyMatrix* m1, const MyMatrix* m2, const MyMatrix* b) {
	_nR = m1->_nR;
	_nC = m2->_nC;
	allocateSpace();
	Formula(m1, m2, b);
}

MyMatrix::MyMatrix(const MyMatrix* m1, bool bTranspose) {
	if (bTranspose)
	{
		_nR = m1->_nC;
		_nC = m1->_nR;
	}
	else {
		_nR = m1->_nR;
		_nC = m1->_nC;
	}
	allocateSpace();
	for (size_t i = 0; i < _nR; i++)
	{
		for (size_t j = 0; j < _nC; j++)
		{
			if (bTranspose)
			{
				_data[i][j] = m1->GetValue(j, i);
			}
			else {
				_data[i][j] = m1->GetValue(i, j);
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

double MyMatrix::Norm2() {
	double dbNorm = 0;
	for (size_t i = 0; i < _nR; i++)
	{
		for (size_t j = 0; j < _nC; j++) {
			dbNorm += _data[i][j] * _data[i][j];
		}
	}
	return dbNorm;
}


void MyMatrix::Multi(const MyMatrix* m1, const MyMatrix* m2) {
	if (m1->_nC == m2->_nR)
	{
		for (size_t i = 0; i < _nR; i++)
		{
			for (size_t j = 0; j < _nC; j++)
			{
				_data[i][j] = 0;
				for (size_t k = 0; k < m1->_nC; k++) {
					_data[i][j] += m1->GetValue(i, k)*m2->GetValue(k, j);
				}
			}
		}
	}
}

void MyMatrix::Formula(const MyMatrix* m1, const MyMatrix* m2, const MyMatrix* b) {
	if (m1->_nC == m2->_nR&&m2->_nC == b->_nC)
	{
		for (size_t i = 0; i < _nR; i++)
		{
			for (size_t j = 0; j < _nC; j++)
			{
				_data[i][j] = b->GetValue(0, j);
				for (size_t k = 0; k < m1->_nC; k++) {
					_data[i][j] += m1->GetValue(i, k)*m2->GetValue(k, j);
				}
			}
		}
	}
}


void MyMatrix::Linear(const MyMatrix* m1, double dbScale) {
	assert(m1->_nR == _nR&&m1->_nC == _nC);
	for (size_t i = 0; i < _nR; i++)
	{
		for (size_t j = 0; j < _nC; j++) {
			_data[i][j] += m1->_data[i][j] * dbScale;
		}
	}
}

// initialize value randomly
void MyMatrix::InitRandom() {
	for (size_t i = 0; i < _nR; i++)
	{
		for (size_t j = 0; j < _nC; j++) {
			_data[i][j] = rand() / (double)RAND_MAX;
		}
	}
}
// initialize value to 0
void MyMatrix::InitZero() {
	for (size_t i = 0; i < _nR; i++)
	{
		for (size_t j = 0; j < _nC; j++) {
			_data[i][j] = 0;
		}
	}

}