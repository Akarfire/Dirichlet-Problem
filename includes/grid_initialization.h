#pragma once
#include <vector>

using MatrixType = std::vector<std::vector<double>>;

class GridInitializationInterpolationX
{
private:
    GridInitializationInterpolationX() {}

public:
    static void initialize(MatrixType& grid) 
    {
        if (grid.empty()) return;
        if (grid[0].empty()) return;

        unsigned n = grid.size() - 1;
        unsigned m = grid[0].size() - 1;

        for (unsigned i = 1; i < n; i++)
            for (unsigned j = 1; j < m; j++)
                grid[i][j] = grid[0][j] + i * (grid[n][j] - grid[0][j]) / n;
    }
};


class GridInitializationInterpolationY
{
private:
    GridInitializationInterpolationY() {}

public:
    static void initialize(MatrixType& grid) 
    {
        if (grid.empty()) return;
        if (grid[0].empty()) return;

        unsigned n = grid.size() - 1;
        unsigned m = grid[0].size() - 1;

        for (unsigned i = 1; i < n; i++)
            for (unsigned j = 1; j < m; j++)
                grid[i][j] = grid[i][0] + j * (grid[i][m] - grid[i][0]) / m;
    }
};


class GridInitializationInterpolationXY
{
private:
    GridInitializationInterpolationXY() {}

public:
    static void initialize(MatrixType& grid) 
    {
        if (grid.empty()) return;
        if (grid[0].empty()) return;

        unsigned n = grid.size() - 1;
        unsigned m = grid[0].size() - 1;

        for (unsigned i = 1; i < n; i++)
            for (unsigned j = 1; j < m; j++)
            {
                double x_interpolation = grid[0][j] + i * (grid[n][j] - grid[0][j]) / n;
                double y_interpolation = grid[i][0] + j * (grid[i][m] - grid[i][0]) / m;

                grid[i][j] = (x_interpolation + y_interpolation) / 2.0;
            }
    }
};