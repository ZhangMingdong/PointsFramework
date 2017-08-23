#pragma once
/*
	Math functions used in this project
	Mingdong
	2017/06/21
*/

#include "tnt_array1d.h"
#include "tnt_array2d.h"

#include "jama_eig.h"

//����һ��չ������|A|
double getA(const Array2D<double>& arcs, int n);

//����ÿһ��ÿһ�е�ÿ��Ԫ������Ӧ������ʽ�����A*
void  getAStart(const Array2D<double>& arcs, int n, Array2D<double>& ans);

// �õ���������src������󱣴浽des�С�
bool GetMatrixInverse(const Array2D<double>& src, int n, Array2D<double>& des);

// �Լ�ʵ�ֵķ�����ʹ�ó��ȱ任
void GetMatrixInverse_2(const Array2D<double>& src, int n, Array2D<double>& des);

template < class T>
void convert_from_string(T& value, const string& s);

void adjust_data(Array2D<double>& d, Array1D<double>& means);

double compute_covariance(const Array2D<double>& d, int i, int j);

void compute_covariance_matrix(const Array2D<double> & d, Array2D<double> & covar_matrix);

// Calculate the eigenvectors and eigenvalues of the covariance
// matrix
void eigen(const Array2D<double> & covar_matrix, Array2D<double>& eigenvector, Array2D<double>& eigenvalue);


void transpose(const Array2D<double>& src, Array2D<double>& target);

// z = x * y
void multiply(const Array2D<double>& x, const Array2D<double>& y, Array2D<double>& z);

