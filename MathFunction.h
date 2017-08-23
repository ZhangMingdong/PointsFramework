#pragma once
/*
	Math functions used in this project
	Mingdong
	2017/06/21
*/

#include "tnt_array1d.h"
#include "tnt_array2d.h"

#include "jama_eig.h"

//按第一行展开计算|A|
double getA(const Array2D<double>& arcs, int n);

//计算每一行每一列的每个元素所对应的余子式，组成A*
void  getAStart(const Array2D<double>& arcs, int n, Array2D<double>& ans);

// 得到给定矩阵src的逆矩阵保存到des中。
bool GetMatrixInverse(const Array2D<double>& src, int n, Array2D<double>& des);

// 自己实现的方法，使用初等变换
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

