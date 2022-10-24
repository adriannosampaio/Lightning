#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>

#include <Eigen/Dense>

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
        .def(py::init<>())
        .def_readwrite("noise", &DomainParameters::noise)
        .def_readwrite("new_points_per_leader", &DomainParameters::new_points_per_leader)
        .def_readwrite("maximum_accepted_candidates", &DomainParameters::maximum_accepted_candidates)
        .def_readwrite("minimum_candidate_distance", &DomainParameters::minimum_candidate_distance)
        .def_readwrite("lightning_segment_size", &DomainParameters::lightning_segment_size)
        .def_readwrite("accepted_distance_to_end_point", &DomainParameters::accepted_distance_to_end_point)
        .def_readwrite("max_iterations", &DomainParameters::max_iterations);

    py::class_<core::ElectricCharge>(m, "ElectricCharge")
        .def(py::init<Eigen::Vector3d, bool>());

    py::class_<PathPoint>(m, "PathPoint")
        .def_readonly("position", &PathPoint::position)
        .def_readonly("parent", &PathPoint::parent)
        .def_readonly("children", &PathPoint::children)
        .def_readonly("level", &PathPoint::level)
        .def_readonly("is_main_branch", &PathPoint::is_main_branch);
    
    py::class_<Lightning>(m, "Lightning")
        .def("get_root", &Lightning::get_root);
    
}