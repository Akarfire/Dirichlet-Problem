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

    class Solution {
        FFuncType f;
        double a, b;
        double c, d;
        double x_step;
        double y_step;
        BoundaryFuncType mu1, mu2;
        BoundaryFuncType mu3, mu4;
        unsigned n, m;
        unsigned ITERMAX;
        
        inline double getX(unsigned ind);
        inline double getY(unsigned ind);
    public:
        Solution(FFuncType f_,
                 double a_, double b_, 
                 double c_, double d_,
                 BoundaryFuncType mu1_, BoundaryFuncType mu2_, 
                 BoundaryFuncType mu3_, BoundaryFuncType mu4_,
                 unsigned n_, unsigned m_,
                 unsigned ITERMAX_) : 
                 f(f_), a(a_), b(b_), c(c_), d(d_), mu1(mu1_), mu2(mu2_), mu3(mu3_), mu4(mu4_), n(n_), m(m_), ITERMAX(ITERMAX_) {}

        std::vector<std::vector<double>> solve();
    };




public:

    // Static solver methods

    static std::vector<std::vector<double>> solveSeidelMethod(FFuncType f,
                                                 double a, double b, 
                                                 double c, double d,
                                                 BoundaryFuncType mu1, BoundaryFuncType mu2, 
                                                 BoundaryFuncType mu3, BoundaryFuncType mu4,
                                                 unsigned n, unsigned m, unsigned NMAX);
};