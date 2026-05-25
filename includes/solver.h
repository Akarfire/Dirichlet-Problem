#pragma once
#include <vector>
#include <tuple>
#include <functional>
#include <memory>

#include <iostream>

#include "error_evaluation.h"
#include "grid_initialization.h"
#include "types.h"
#include "solutions.h"

class Solver final
{
private:
    Solver() {}
    
public:
    // Static solver methods
    
    static Matrix2DType getInitialApproximation( double a, double b, 
                                                double c, double d,
                                                BoundaryFuncType mu1, BoundaryFuncType mu2, 
                                                BoundaryFuncType mu3, BoundaryFuncType mu4,
                                                unsigned n, unsigned m)
    {
        MatrixType V((n + 1) * (m + 1));
        GridInitializationInterpolationXY::initialize(V, a, b, c, d, mu1, mu2, mu3, mu4, n, m);
        return convertLinearMatrixTo2D(V, n, m);
    }

    static std::tuple<Matrix2DType, unsigned, double> solveSeidelMethodTest(
                                                 FFuncType analytical, FFuncType f,
                                                 double a, double b, 
                                                 double c, double d,
                                                 BoundaryFuncType mu1, BoundaryFuncType mu2, 
                                                 BoundaryFuncType mu3, BoundaryFuncType mu4,
                                                 unsigned n, unsigned m,
                                                 unsigned NMAX, double epsilon)
    {
        SeidelSolution<AnalyticalErrorEvaluation, GridInitializationInterpolationXY> Solution(f, a, b, c, d, mu1, mu2, mu3, mu4, n, m, NMAX);
        Solution.setupErrorEvaluation(epsilon, analytical);

        auto result = Solution.solve();
        return {std::get<0>(result), std::get<1>(result), Solution.getLastStepError()};
    }
    
    static std::tuple<Matrix2DType, unsigned, double> solveSeidelMethodMain(
                                                 FFuncType f,
                                                 double a, double b, 
                                                 double c, double d,
                                                 BoundaryFuncType mu1, BoundaryFuncType mu2, 
                                                 BoundaryFuncType mu3, BoundaryFuncType mu4,
                                                 unsigned n, unsigned m,
                                                 unsigned NMAX, double epsilon)
    {
        SeidelSolution<HeuristicErrorEvaluation, GridInitializationInterpolationXY> Solution(f, a, b, c, d, mu1, mu2, mu3, mu4, n, m, NMAX);
        Solution.setupErrorEvaluation(epsilon);

        auto result = Solution.solve();
        return {std::get<0>(result), std::get<1>(result), Solution.getLastStepError()};
    }     

    static std::tuple<Matrix2DType, unsigned, double> solveRelaxMethodTest(
                                                FFuncType analytical, FFuncType f,
                                                double a, double b, 
                                                double c, double d,
                                                BoundaryFuncType mu1, BoundaryFuncType mu2, 
                                                BoundaryFuncType mu3, BoundaryFuncType mu4,
                                                unsigned n, unsigned m,
                                                unsigned NMAX, double epsilon, double omega)
    {
        RelaxSolution<AnalyticalErrorEvaluation, GridInitializationInterpolationXY> Solution(f, a, b, c, d, mu1, mu2, mu3, mu4, n, m, NMAX, omega);
        Solution.setupErrorEvaluation(epsilon, analytical);

        auto result = Solution.solve();
        return {std::get<0>(result), std::get<1>(result), Solution.getLastStepError()};
    }
                                                 
    static std::tuple<Matrix2DType, unsigned, double> solveRelaxMethodMain(
                                                 FFuncType f,
                                                 double a, double b, 
                                                 double c, double d,
                                                 BoundaryFuncType mu1, BoundaryFuncType mu2, 
                                                 BoundaryFuncType mu3, BoundaryFuncType mu4,
                                                 unsigned n, unsigned m,
                                                 unsigned NMAX, double epsilon, double omega)
    {
        RelaxSolution<HeuristicErrorEvaluation, GridInitializationInterpolationXY> Solution(f, a, b, c, d, mu1, mu2, mu3, mu4, n, m, NMAX, omega);
        Solution.setupErrorEvaluation(epsilon);

        auto result = Solution.solve();
        return {std::get<0>(result), std::get<1>(result), Solution.getLastStepError()};
    }
};