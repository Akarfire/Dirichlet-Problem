#include <iostream>
#include "solver.h"
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include <pybind11/functional.h>


// Create the pybind11 module
PYBIND11_MODULE(dirichletsolver, m) {
    m.doc() = "Solves Dirichlet problem for Poissons equation";
    
    m.def("solve_seidel_method_test", &Solver::solveSeidelMethodTest,
        pybind11::arg("analytical"), pybind11::arg("f"), 
        pybind11::arg("a"), pybind11::arg("b"), pybind11::arg("c"), pybind11::arg("d"),
        pybind11::arg("mu1"), pybind11::arg("mu2"), pybind11::arg("mu3"), pybind11::arg("mu4"),
        pybind11::arg("n"), pybind11::arg("m"),
        pybind11::arg("NMAX"), pybind11::arg("epsilon")
    );

    m.def("solve_seidel_method_main", &Solver::solveSeidelMethodMain,
        pybind11::arg("f"), 
        pybind11::arg("a"), pybind11::arg("b"), pybind11::arg("c"), pybind11::arg("d"),
        pybind11::arg("mu1"), pybind11::arg("mu2"), pybind11::arg("mu3"), pybind11::arg("mu4"),
        pybind11::arg("n"), pybind11::arg("m"),
        pybind11::arg("NMAX"), pybind11::arg("epsilon")
    );

    m.def("solve_relax_method_test", &Solver::solveRelaxMethodTest,
        pybind11::arg("analytical"), pybind11::arg("f"), 
        pybind11::arg("a"), pybind11::arg("b"), pybind11::arg("c"), pybind11::arg("d"),
        pybind11::arg("mu1"), pybind11::arg("mu2"), pybind11::arg("mu3"), pybind11::arg("mu4"),
        pybind11::arg("n"), pybind11::arg("m"),
        pybind11::arg("NMAX"), pybind11::arg("epsilon"), pybind11::arg("omega")
    );

    m.def("solve_relax_method_main", &Solver::solveRelaxMethodMain,
        pybind11::arg("f"), 
        pybind11::arg("a"), pybind11::arg("b"), pybind11::arg("c"), pybind11::arg("d"),
        pybind11::arg("mu1"), pybind11::arg("mu2"), pybind11::arg("mu3"), pybind11::arg("mu4"),
        pybind11::arg("n"), pybind11::arg("m"),
        pybind11::arg("NMAX"), pybind11::arg("epsilon"), pybind11::arg("omega")
    );
}
