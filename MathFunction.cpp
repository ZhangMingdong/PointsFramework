#include "MathFunction.h"
#include <qdebug.h>
#include <assert.h>

using namespace TNT;
using namespace JAMA;
using namespace std;

//按第一行展开计算|A|
double getA(const Array2D<double>& arcs, int n)
{
	if (n == 1)
	{
		return arcs[0][0];
	}
	double ans = 0;
	Array2D<double> temp(n, n);
	int i, j, k;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n - 1; j++)
		{
			for (k = 0; k < n - 1; k++)
			{
				temp[j][k] = arcs[j + 1][(k >= i) ? k + 1 : k];

			}
		}
		double t = getA(temp, n - 1);
		if (i % 2 == 0)
		{
			ans += arcs[0][i] * t;
		}
		else
		{
			ans -= arcs[0][i] * t;
		}
	}
	return ans;
}

//计算每一行每一列的每个元素所对应的余子式，组成A*
void  getAStart(const Array2D<double>& arcs, int n, Array2D<double>& ans)
{
	if (n == 1)
	{
		ans[0][0] = 1;
		return;
	}
	int i, j, k, t;
	Array2D<double> temp(n, n);
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			for (k = 0; k < n - 1; k++)
			{
				for (t = 0; t < n - 1; t++)
				{
					temp[k][t] = arcs[k >= i ? k + 1 : k][t >= j ? t + 1 : t];
				}
			}


			ans[j][i] = getA(temp, n - 1);
			if ((i + j) % 2 == 1)
			{
				ans[j][i] = -ans[j][i];
			}
		}
	}
}

// 得到给定矩阵src的逆矩阵保存到des中。
bool GetMatrixInverse(const Array2D<double>& src, int n, Array2D<double>& des)
{
	double flag = getA(src, n);

	Array2D<double> t(n, n);
	if (flag == 0)
	{
		return false;
	}
	else
	{
		getAStart(src, n, t);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				des[i][j] = t[i][j] / flag;
			}

		}
	}


	return true;

}

// 自己实现的方法，使用初等变换
void GetMatrixInverse_2(const Array2D<double>& src, int n, Array2D<double>& des) {
	// 0.initialized des to E
	Array2D<double> srcTemp(n, n);
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			if (i == j) des[i][j] = 1;
			else des[i][j] = 0;

			srcTemp[i][j] = src[i][j];
		}
	}

	// 2.up down minus
	for (size_t i = 0; i < n; i++)
	{

		for (size_t k = 0; k < i; k++)		// each line before this line
		{
			double dbScale = srcTemp[i][k];
			for (size_t j = 0; j < n; j++)
			{
				srcTemp[i][j] -= (srcTemp[k][j] * dbScale);
				des[i][j] -= (des[k][j] * dbScale);
			}
		}
		if (srcTemp[i][i] == 0)
		{
			int k = i + 1;
			while (k < n&&srcTemp[k][i] == 0) k++;
			Q_ASSERT(k < n);							// 否则局针不满秩

														// swap the two lines
			for (size_t l = 0; l < n; l++)
			{
				double temp = srcTemp[i][l];
				srcTemp[i][l] = srcTemp[k][l];
				srcTemp[k][l] = temp;
				temp = des[i][l];
				des[i][l] = des[k][l];
				des[k][l] = temp;
			}



		}
		// 1.normalize
		double dbScale = srcTemp[i][i];
		for (size_t j = 0; j < n; j++)
		{
			srcTemp[i][j] /= dbScale;
			des[i][j] /= dbScale;
		}
	}
	// 3.bottom up minus
	for (int i = n - 2; i >= 0; i--)
	{
		for (size_t k = n - 1; k > i; k--)
		{
			double dbScale = srcTemp[i][k];
			for (size_t j = 0; j < n; j++)
			{
				srcTemp[i][j] -= (srcTemp[k][j] * dbScale);
				des[i][j] -= (des[k][j] * dbScale);
			}
		}
	}
}

template < class T>
void convert_from_string(T& value, const string& s)
{
	stringstream ss(s);
	ss >> value;
}

void adjust_data(Array2D<double>& d, Array1D<double>& means) {
	for (int i = 0; i < d.dim2(); ++i) {
		double mean = 0;
		for (int j = 0; j < d.dim1(); ++j) {
			mean += d[j][i];
		}

		mean /= d.dim1();

		// store the mean
		means[i] = mean;

		// subtract the mean
		for (int j = 0; j < d.dim1(); ++j) {
			d[j][i] -= mean;
		}
	}
}

double compute_covariance(const Array2D<double>& d, int i, int j) {
	double cov = 0;
	for (int k = 0; k < d.dim1(); ++k) {
		cov += d[k][i] * d[k][j];
	}

	return cov / (d.dim1() - 1);
}

void compute_covariance_matrix(const Array2D<double> & d, Array2D<double> & covar_matrix) {
	int dim = d.dim2();
	assert(dim == covar_matrix.dim1());
	assert(dim == covar_matrix.dim2());
	for (int i = 0; i < dim; ++i) {
		for (int j = i; j < dim; ++j) {
			covar_matrix[i][j] = compute_covariance(d, i, j);
		}
	}


	// fill the Left triangular matrix
	for (int i = 1; i < dim; i++) {
		for (int j = 0; j < i; ++j) {
			covar_matrix[i][j] = covar_matrix[j][i];
		}
	}

}

// Calculate the eigenvectors and eigenvalues of the covariance
// matrix
void eigen(const Array2D<double> & covar_matrix, Array2D<double>& eigenvector, Array2D<double>& eigenvalue) {
	Eigenvalue<double> eig(covar_matrix);
	eig.getV(eigenvector);
	eig.getD(eigenvalue);
}


void transpose(const Array2D<double>& src, Array2D<double>& target) {
	for (int i = 0; i < src.dim1(); ++i) {
		for (int j = 0; j < src.dim2(); ++j) {
			target[j][i] = src[i][j];
		}
	}
}

// z = x * y
void multiply(const Array2D<double>& x, const Array2D<double>& y, Array2D<double>& z) {
	assert(x.dim2() == y.dim1());
	for (int i = 0; i < x.dim1(); ++i) {
		for (int j = 0; j < y.dim2(); ++j) {
			double sum = 0;
			int d = y.dim1();
			for (int k = 0; k < d; k++) {
				sum += x[i][k] * y[k][j];
			}
			z[i][j] = sum;
		}
	}
}
