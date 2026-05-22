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

        const double inv_n = 1.0 / n;
        const double inv_m = 1.0 / m;

        for (unsigned i = 1; i < n; i++) {
            const double alpha = i * inv_n;
            const double one_minus_alpha = 1.0 - alpha;
            double* row_i = grid[i].data();
            
            for (unsigned j = 1; j < m; j++) {
                const double beta = j * inv_m;
                const double one_minus_beta = 1.0 - beta;
                
                row_i[j] = one_minus_beta * (one_minus_alpha * grid[0][0] + alpha * grid[n][0]) +
                            beta * (one_minus_alpha * grid[0][m] + alpha * grid[n][m]);
            }
        }
    }
};