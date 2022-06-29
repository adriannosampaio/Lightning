#pragma once
#include <Eigen/Dense>
#include <vector>

#include "Constants.hpp"
#include "DomainParameters.hpp"
#include "ElectricCharge.hpp"

namespace physics {
class ElectricField {
   private:
    std::vector<double> _electric_potential_cells;
    const DomainParameters& _parameters;

   public:
    explicit ElectricField(const DomainParameters& parameters) :
        _parameters(parameters),
        _electric_potential_cells(
            std::vector<double>(parameters.number_of_cells.prod())) {}

    void generate_field(const std::vector<physics::ElectricCharge>& charges) {
        // Calculate electric potential
        for (int cell_idx = 0; cell_idx < _electric_potential_cells.size();
             cell_idx++) {
            _electric_potential_cells[cell_idx] = 0.0;
            auto cell_center = _parameters.cell_index_to_cell_center(cell_idx);
            for (auto& charge : charges) {
                double distance_from_cell =
                    (cell_center - charge.position).norm();
                _electric_potential_cells[cell_idx] +=
                    (charge.charge / distance_from_cell);
            }
            _electric_potential_cells[cell_idx] *= physics::COULUMBS_CONSTANT;
        }
    }

    double get_potential(const Eigen::Vector3d& position) { return 0.0; }

    ~ElectricField() {}
};

}  // namespace physics
