#pragma once
#include <vector>
#include <functional>

using FFuncType = std::function<double(double, double)>;
using BoundaryFuncType = std::function<double(double)>;
using MatrixType = std::vector<double>;
using Matrix2DType = std::vector<std::vector<double>>;

static inline unsigned index(unsigned i, unsigned j, unsigned m)
{
    return i * (m + 1) + j;
}

static inline Matrix2DType convertLinearMatrixTo2D(const MatrixType& linearMatrix, unsigned n, unsigned m)
{
    Matrix2DType result;

    result.resize(n + 1);
    for (unsigned i = 0; i < n + 1; i++)
    {
        result[i].resize(m + 1);
        for (unsigned j = 0; j < m + 1; j++)
            result[i][j] = linearMatrix[index(i, j, m)];
    }

    return result;
}