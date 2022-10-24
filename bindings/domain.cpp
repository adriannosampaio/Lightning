#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>

#include "Domain.hpp"
#include "DomainParameters.hpp"
#include "ElectricCharge.hpp"
#include "Lightning.hpp"
#include "PathPoint.hpp"

namespace py = pybind11;


PYBIND11_MODULE(pyLightning, m) {
    py::class_<Domain>(m, "Domain")
        .def(py::init<>())
        .def("add_charge", &Domain::add_charge)
        .def("generate_path", &Domain::generate_path);

        
    py::class_<DomainParameters>(m, "DomainParameters")
        .def(py::init<>());
    
}