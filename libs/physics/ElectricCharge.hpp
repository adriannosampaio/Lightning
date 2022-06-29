#pragma once
#include <Eigen/Dense>

namespace physics {

struct ElectricCharge {
    Eigen::Vector3d position;
    double charge;

    ElectricCharge(Eigen::Vector3d position, double charge) :
        position(position), charge(charge) {}

    ~ElectricCharge() {}
};

}  // namespace physics