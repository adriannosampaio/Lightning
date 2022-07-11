#pragma once
#include <Eigen/Dense>

#include "Utils.hpp"
namespace physics {

struct ElectricCharge {
    Eigen::Vector3d position;
    double charge;

    ElectricCharge(Eigen::Vector3d position, double charge) :
        position(position), charge(charge) {}

    inline double get_electric_potential(
        const Eigen::Vector3d& affected_position) const {
        double distance_from_cell = (affected_position - position).norm();
        return physics::COULUMBS_CONSTANT * (charge / distance_from_cell);
    }

    ~ElectricCharge() {}
};

}  // namespace physics