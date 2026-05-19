#pragma once
#include <vector>
#include <tuple>
#include <functional>

using FFuncType = std::function<double(double, double)>;
using BoundaryFuncType = std::function<double(double)>;

class Solver final
{
private:
    Solver() {}

public:

    // Static solver methods

    static std::vector<double> solveSeidelMethod(FFuncType f,
                                                 double a, double b, 
                                                 double c, double d,
                                                 BoundaryFuncType mu1, BoundaryFuncType mu2, 
                                                 BoundaryFuncType mu3, BoundaryFuncType mu4,
                                                 unsigned n, unsigned m);
};