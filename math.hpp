#pragma once
#include <array>

namespace math
{
inline std::array<double, 3> cross(double* __restrict__ a, double* __restrict__ b)
{
	return {
		a[2]*b[3] - a[3]*b[2],
		a[3]*b[1] - a[1]*b[3],
		a[1]*b[2] - a[2]*b[1]
	};
}

inline double dot(double* __restrict__ a, double* __restrict__ b)
{
	return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}
}
