#pragma once
#include <vector>
#include "types.h"


class BaseGridInitialization
{
protected:
    BaseGridInitialization() {}

public:
    static void initializeBoundaries(MatrixType& grid,  double a, double b, 
                                                        double c, double d,
                                                        BoundaryFuncType mu1, BoundaryFuncType mu2, 
                                                        BoundaryFuncType mu3, BoundaryFuncType mu4,
                                                        unsigned n, unsigned m)
    {
        double x_step = (b - a) / n;
        double y_step = (d - c) / m;

        for (unsigned i = 0; i < n + 1; i++) 
        {
            grid[index(i, 0, m)] = mu3(a + i * x_step); 
            grid[index(i, m, m)] = mu4(a + i * x_step);
        }

        for (unsigned j = 1; j < m + 1; j++) 
        {
            grid[index(0, j, m)] = mu1(c + j * y_step);
            grid[index(n, j, m)] = mu2(c + j * y_step);
        }
    }
};


class GridInitializationInterpolationX: public BaseGridInitialization
{
private:
    GridInitializationInterpolationX() {}

public:
    static void initialize(MatrixType& grid, double a, double b, 
                                             double c, double d,
                                             BoundaryFuncType mu1, BoundaryFuncType mu2, 
                                             BoundaryFuncType mu3, BoundaryFuncType mu4,
                                             unsigned n, unsigned m)
    {
        if (grid.empty()) return;

        initializeBoundaries(grid, a, b, c, d, mu1, mu2, mu3, mu4, n, m);

        for (unsigned i = 1; i < n; i++)
            for (unsigned j = 1; j < m; j++)
                grid[index(i, j, m)] = grid[index(0, j, m)] + i * (grid[index(n, j, m)] - grid[index(0, j, m)]) / n;
    }
};


class GridInitializationInterpolationY: public BaseGridInitialization
{
private:
    GridInitializationInterpolationY() {}

public:
    static void initialize(MatrixType& grid, double a, double b, 
                                             double c, double d,
                                             BoundaryFuncType mu1, BoundaryFuncType mu2, 
                                             BoundaryFuncType mu3, BoundaryFuncType mu4,
                                             unsigned n, unsigned m)
    {
        if (grid.empty()) return;

        initializeBoundaries(grid, a, b, c, d, mu1, mu2, mu3, mu4, n, m);

        for (unsigned i = 1; i < n; i++)
            for (unsigned j = 1; j < m; j++)
                grid[index(i, j, m)] = grid[index(i, 0, m)] + j * (grid[index(i, m, m)] - grid[index(i, 0, m)]) / m;
    }
};


class GridInitializationInterpolationXY: public BaseGridInitialization
{
private:
    GridInitializationInterpolationXY() {}

public:
    static void initialize(MatrixType& grid, double a, double b, 
                                             double c, double d,
                                             BoundaryFuncType mu1, BoundaryFuncType mu2, 
                                             BoundaryFuncType mu3, BoundaryFuncType mu4,
                                             unsigned n, unsigned m)
    {
        if (grid.empty()) return;

        initializeBoundaries(grid, a, b, c, d, mu1, mu2, mu3, mu4, n, m);

        const double inv_n = 1.0 / n;
        const double inv_m = 1.0 / m;

        for (unsigned i = 1; i < n; i++)
            for (unsigned j = 1; j < m; j++)
            {
                double x_weight = std::max((double)i / n, (double)(n - i) / n);
                double y_weight = std::max((double)j / m, (double)(m - j) / m);
                double x_interpolation = grid[index(0, j, m)] + i * (grid[index(n, j, m)] - grid[index(0, j, m)]) / n;
                double y_interpolation = grid[index(i, 0, m)] + j * (grid[index(i, m, m)] - grid[index(i, 0, m)]) / m;
                grid[index(i, j, m)] = x_weight * x_interpolation + y_weight * y_interpolation;
            }
    }
};