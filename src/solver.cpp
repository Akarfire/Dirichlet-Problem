#include "solver.h"

inline double Solver::SeidelSolution::getX(unsigned ind) {
    return a + ind * x_step;
}

inline double Solver::SeidelSolution::getY(unsigned ind) {
    return b + ind * y_step;
}

std::vector<std::vector<double>> Solver::SeidelSolution::solve() {
    x_step = (b - a) / n;
    y_step = (d - c) / m;

    const double inv_x_step_sq = 1 / (x_step * x_step);
    const double inv_y_step_sq = 1 / (y_step * y_step);
    const double A = -2 * (inv_x_step_sq + inv_y_step_sq);
    const double inv_A = 1 / A;

    std::vector<std::vector<double>> V(n + 1);

    for (unsigned i = 0; i < n + 1; i++) {
        V[i].resize(m + 1);
        V[i][0] = mu3(getX(i)); 
        V[i][m] = mu4(getX(i));
    }

    for (unsigned j = 1; j < m - 1; j++) {
        V[0][j] = mu1(getY(j));
        V[n][j] = mu2(getY(j));
    }

    for (unsigned iteration = 1; iteration <= ITERMAX; iteration++) {
        for (unsigned j = 1; j < m; j++) {
            for (unsigned i = 1; i < n; i++) {
                V[i][j] = inv_A * (
                                - f(getX(i), getY(j))
                                - inv_x_step_sq * V[i - 1][j]
                                - inv_x_step_sq * V[i + 1][j]
                                - inv_y_step_sq * V[i][j - 1]
                                - inv_y_step_sq * V[i][j + 1]);
            }
        }
    }

    return V;
}

std::vector<std::vector<double>> Solver::solveSeidelMethod(FFuncType f,
                                              double a, double b, 
                                              double c, double d,
                                              BoundaryFuncType mu1, BoundaryFuncType mu2, 
                                              BoundaryFuncType mu3, BoundaryFuncType mu4,
                                              unsigned n, unsigned m,
                                              unsigned NMAX)
{
    SeidelSolution SeidelSolution(f, a, b, c, d, mu1, mu2, mu3, mu4, n, m, NMAX);
    return SeidelSolution.solve();
}
