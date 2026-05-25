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
    
    static std::tuple<Matrix2DType, unsigned> solveSeidelMethodTest(
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
        SeidelSolution<HeuristicErrorEvaluation, GridInitializationInterpolationXY> Solution(f, a, b, c, d, mu1, mu2, mu3, mu4, n, m, NMAX);
        Solution.setupErrorEvaluation(epsilon);

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
        RelaxSolution<AnalyticalErrorEvaluation, GridInitializationInterpolationXY> Solution(f, a, b, c, d, mu1, mu2, mu3, mu4, n, m, NMAX, omega);
        Solution.setupErrorEvaluation(epsilon, analytical);

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
        RelaxSolution<HeuristicErrorEvaluation, GridInitializationInterpolationXY> Solution(f, a, b, c, d, mu1, mu2, mu3, mu4, n, m, NMAX, omega);
        Solution.setupErrorEvaluation(epsilon);

        return Solution.solve();
    }
};