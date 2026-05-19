#include <iostream>
#include "solver.h"
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include <pybind11/functional.h>

// Create the pybind11 module
PYBIND11_MODULE(dirichletsolver, m) {
    m.doc() = "Solves Dirichlet problem for Poissons equation";
    
    // m.def("solve", &pySolve,
    //     pybind11::arg("k1_func"), ...
    // );
}