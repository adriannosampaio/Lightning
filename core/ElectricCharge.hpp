#pragma once
#include <Eigen/Dense>

#include "Utils.hpp"
namespace core {

struct ElectricCharge {
    Eigen::Vector3d position;
    double charge;

    ElectricCharge(Eigen::Vector3d position, double charge) :
        position(position), charge(charge) {}

    inline const Eigen::Vector3d& get_position() { return position; }
    inline double get_electric_potential(
        const Eigen::Vector3d& affected_position) const {
        double distance_from_cell = (affected_position - position).norm();
        return core::COULUMBS_CONSTANT * (charge / distance_from_cell);
    }

    ~ElectricCharge() {}
};

}  // namespace physics