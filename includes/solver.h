#pragma once
#include <vector>
#include <tuple>
#include <functional>
#include <memory>

#include <iostream>

#include "error_evaluation.h"
#include "grid_initialization.h"
#include "types.h"


class Solver final
{
private:
    Solver() {}

    enum class Method {
        Seidel,
        Relax
    };

    template<Method method, class ErrorEvaluationType, class GridInitializationType>
    class Solution 
    {
    private:
        std::shared_ptr<ErrorEvaluationType> errorEval;
        FFuncType f;
        double a, b;
        double c, d;
        double x_step; // h
        double y_step; // k
        BoundaryFuncType mu1, mu2;
        BoundaryFuncType mu3, mu4;
        unsigned n, m;
        unsigned ITERMAX;

        MatrixType f_cache;
 
        double omega = 1.0;
   
        double inv_x_step_sq, inv_y_step_sq, A, inv_A;

        inline double getX(unsigned ind) {
            return a + ind * x_step;
        }
        inline double getY(unsigned ind) {
            return c + ind * y_step;
        }

        inline double relaxComputation(unsigned i, unsigned j, double x, double y, MatrixType& V) {
            return -1.0 * inv_A * ( (1 - omega) * (-1 * A) * V[index(i, j, m)] + 
                omega * (inv_x_step_sq * V[index(i - 1, j, m)] + inv_x_step_sq * V[index(i + 1, j, m)] 
                       + inv_y_step_sq * V[index(i, j - 1, m)] + inv_y_step_sq * V[index(i, j + 1, m)]
                       + f_cache[index(i, j, m)]));
        }
        inline double seidelComputation(unsigned i, unsigned j, double x, double y, MatrixType& V) {
                return inv_A * (- f_cache[index(i, j, m)] - inv_x_step_sq * V[index(i - 1, j, m)] - inv_x_step_sq * V[index(i + 1, j, m)] 
                    - inv_y_step_sq * V[index(i, j - 1, m)] - inv_y_step_sq * V[index(i, j + 1, m)]);
        }

    public:
        Solution(FFuncType f_,
                 double a_, double b_, 
                 double c_, double d_,
                 BoundaryFuncType mu1_, BoundaryFuncType mu2_, 
                 BoundaryFuncType mu3_, BoundaryFuncType mu4_,
                 unsigned n_, unsigned m_,
                 unsigned ITERMAX_, std::shared_ptr<ErrorEvaluationType> errorEvaluation, double omega_ = 1.0) : 
                 f(f_), a(a_), b(b_), c(c_), d(d_), mu1(mu1_), mu2(mu2_), mu3(mu3_), mu4(mu4_), n(n_), m(m_), 
                 ITERMAX(ITERMAX_), errorEval(errorEvaluation), omega(omega_) {}

        std::tuple<Matrix2DType, unsigned> solve()
        {
            x_step = (b - a) / n;
            y_step = (d - c) / m;

            inv_x_step_sq = 1 / (x_step * x_step);
            inv_y_step_sq = 1 / (y_step * y_step);
            A = -2 * (inv_x_step_sq + inv_y_step_sq);
            inv_A = 1 / A;

            MatrixType V((n + 1) * (m + 1));

            for (unsigned i = 0; i < n + 1; i++) {
                V[index(i, 0, m)] = mu3(getX(i)); 
                V[index(i, m, m)] = mu4(getX(i));
            }

            for (unsigned j = 1; j < m + 1; j++) {
                V[index(0, j, m)] = mu1(getY(j));
                V[index(n, j, m)] = mu2(getY(j));
            }

            f_cache.resize((n + 1) * (m + 1));
            for (unsigned i = 1; i < n; i++) {
                for (unsigned j = 1; j < m; j++) {
                    f_cache[index(i, j, m)] = f(getX(i), getY(j));
                }
            }

            GridInitializationType::initialize(V, n, m);

            unsigned iteration = 1;

            for (iteration = 1; iteration <= ITERMAX; iteration++) {
                for (unsigned j = 1; j < m; j++) {
                    for (unsigned i = 1; i < n; i++) {
                        double x = getX(i);
                        double y = getY(j);
                        
                        errorEval->cacheOld(V[index(i, j, m)], x, y);

                        if constexpr(method == Method::Seidel) {
                            V[index(i, j, m)] = seidelComputation(i, j, x, y, V);
                        }
                        else {
                            V[index(i, j, m)] = relaxComputation(i, j, x, y, V);
                        }

                        errorEval->evaluateNew(V[index(i, j, m)], x, y);
                    }
                }

                if (!errorEval->getEvaluationResult())
                    break;
            }

            return {convertLinearMatrixTo2D(V, n, m), iteration};
        }
    };
public:
    // Static solver methods
    
    static std::tuple<Matrix2DType, unsigned> solveSeidelMethodTest(
                                                 FFuncType analytical, FFuncType f,
                                                 double a, double b, 
                                                 double c, double d,
                                                 BoundaryFuncType mu1, BoundaryFuncType mu2, 
                                                 BoundaryFuncType mu3, BoundaryFuncType mu4,
                                                 unsigned n, unsigned m,
                                                 unsigned NMAX, double epsilon)
    {
        Solution<Method::Seidel, AnalyticalErrorEvaluation, GridInitializationInterpolationXY> Solution(f, a, b, c, d, mu1, mu2, mu3, mu4, n, m, NMAX,
            std::make_shared<AnalyticalErrorEvaluation>(epsilon, analytical));
        
        return Solution.solve();
    }
    
    static std::tuple<Matrix2DType, unsigned> solveSeidelMethodMain(
                                                 FFuncType f,
                                                 double a, double b, 
                                                 double c, double d,
                                                 BoundaryFuncType mu1, BoundaryFuncType mu2, 
                                                 BoundaryFuncType mu3, BoundaryFuncType mu4,
                                                 unsigned n, unsigned m,
                                                 unsigned NMAX, double epsilon)
    {
        Solution<Method::Seidel, HeuristicErrorEvaluation, GridInitializationInterpolationXY> Solution(f, a, b, c, d, mu1, mu2, mu3, mu4, n, m, NMAX,
            std::make_shared<HeuristicErrorEvaluation>(epsilon));
        return Solution.solve();
    }     

    static std::tuple<Matrix2DType, unsigned> solveRelaxMethodTest(
                                                FFuncType analytical, FFuncType f,
                                                double a, double b, 
                                                double c, double d,
                                                BoundaryFuncType mu1, BoundaryFuncType mu2, 
                                                BoundaryFuncType mu3, BoundaryFuncType mu4,
                                                unsigned n, unsigned m,
                                                unsigned NMAX, double epsilon, double omega)
    {
        Solution<Method::Relax, AnalyticalErrorEvaluation, GridInitializationInterpolationXY> Solution(f, a, b, c, d, mu1, mu2, mu3, mu4, n, m, NMAX,
            std::make_shared<AnalyticalErrorEvaluation>(epsilon, analytical), omega);
        return Solution.solve();
    }
                                                 
    static std::tuple<Matrix2DType, unsigned> solveRelaxMethodMain(
                                                 FFuncType f,
                                                 double a, double b, 
                                                 double c, double d,
                                                 BoundaryFuncType mu1, BoundaryFuncType mu2, 
                                                 BoundaryFuncType mu3, BoundaryFuncType mu4,
                                                 unsigned n, unsigned m,
                                                 unsigned NMAX, double epsilon, double omega)
    {
        Solution<Method::Relax, HeuristicErrorEvaluation, GridInitializationInterpolationXY> Solution(f, a, b, c, d, mu1, mu2, mu3, mu4, n, m, NMAX,
            std::make_shared<HeuristicErrorEvaluation>(epsilon), omega);

        return Solution.solve();
    }
};