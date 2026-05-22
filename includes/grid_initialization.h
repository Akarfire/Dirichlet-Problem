#pragma once
#include <vector>
#include "types.h"

class GridInitializationInterpolationX
{
private:
    GridInitializationInterpolationX() {}

public:
    static void initialize(MatrixType& grid, unsigned n, unsigned m) 
    {
        if (grid.empty()) return;

        for (unsigned i = 1; i < n; i++)
            for (unsigned j = 1; j < m; j++)
                grid[index(i, j, m)] = grid[index(0, j, m)] + i * (grid[index(n, j, m)] - grid[index(0, j, m)]) / n;
    }
};


class GridInitializationInterpolationY
{
private:
    GridInitializationInterpolationY() {}

public:
    static void initialize(MatrixType& grid, unsigned n, unsigned m) 
    {
        if (grid.empty()) return;

        for (unsigned i = 1; i < n; i++)
            for (unsigned j = 1; j < m; j++)
                grid[index(i, j, m)] = grid[index(i, 0, m)] + j * (grid[index(i, m, m)] - grid[index(i, 0, m)]) / m;
    }
};


class GridInitializationInterpolationXY
{
private:
    GridInitializationInterpolationXY() {}

public:
    static void initialize(MatrixType& grid, unsigned n, unsigned m) 
    {
        if (grid.empty()) return;

        const double inv_n = 1.0 / n;
        const double inv_m = 1.0 / m;

        for (unsigned i = 1; i < n; i++) {
            const double alpha = i * inv_n;
            const double one_minus_alpha = 1.0 - alpha;
            
            for (unsigned j = 1; j < m; j++) {
                const double beta = j * inv_m;
                const double one_minus_beta = 1.0 - beta;
                
                grid[index(i, j, m)] = one_minus_beta * (one_minus_alpha * grid[index(0, 0, m)] + alpha 
                * grid[index(n, 0, m)]) + beta * (one_minus_alpha * grid[index(0, m, m)] + alpha * grid[index(n, m, m)]);
            }
        }
    }
};