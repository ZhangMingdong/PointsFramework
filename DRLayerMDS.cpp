
#include "DRLayer.h"

#include <Eigen/Core>
#include <mathtoolbox/classical-mds.hpp>
#include <iostream>

using namespace Eigen;

void DRLayer::usingMDS() {

	int n = _vecRaw.size();
	std::vector<VectorXd> points(n);
	for (size_t i = 0; i < n; i++)
	{

		points[i] = VectorXd(_nAttributes); 
		for (size_t j = 0; j < _nAttributes; j++)
		{
			points[i](j)= _vecRaw[i]._arrBuf[j];
		}
	}

	MatrixXd D(n, n);
	for (unsigned i = 0; i < n; ++i)
	{
		for (unsigned j = i; j < n; ++j)
		{
			const double d = (points[i] - points[j]).norm();
			D(i, j) = d;
			D(j, i) = d;
		}
	}

	// Compute metric MDS (embedding into a 2-dimensional space)
	const MatrixXd X = mathtoolbox::ComputeClassicalMds(D, 2);

	for (size_t i = 0; i < n; i++)
	{

		LabeledPoint pt(X(0,i),X(1,i), _vecRaw[i]._nLabel);
		_vecPoints.push_back(pt);
	}

}
