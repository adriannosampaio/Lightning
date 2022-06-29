#pragma once
#include <Eigen/Dense>
#include <vector>

#include "ElectricCharge.hpp"

namespace physics {
class ElectricField {
   private:
    std::vector<double> cell_potentials;

   public:
    ElectricField() {}

    double get_potential(const Eigen::Vector3d& position) { return 0.0; }

    ~ElectricField() {}
};

}  // namespace physics
