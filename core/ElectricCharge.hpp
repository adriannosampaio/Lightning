#pragma once
#include <Eigen/Dense>

#include "Utils.hpp"
namespace core {

struct ElectricCharge {
    Eigen::Vector3d position;
    bool is_negative;

    ElectricCharge(Eigen::Vector3d position, bool is_negative) :
        position(position), is_negative(is_negative) {}

    inline const Eigen::Vector3d& get_position() { return position; }
    ~ElectricCharge() {}
};

}  // namespace physics