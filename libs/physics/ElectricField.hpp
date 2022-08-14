#pragma once
#include <Eigen/Dense>
#include <vector>

#include "DomainParameters.hpp"
#include "ElectricCharge.hpp"
#include "Utils.hpp"

namespace physics {
class ElectricField {
   private:
    std::vector<double> _electric_potential_cells;
    const std::shared_ptr<DomainParameters> _parameters;

   public:
    explicit ElectricField(std::shared_ptr<DomainParameters> parameters) :
        _parameters(parameters),
        _electric_potential_cells(
            std::vector<double>(parameters->number_of_cells.prod())) {}

    void generate_field(const std::vector<physics::ElectricCharge>& charges, double noise_percent=0.2) {
        // Calculate electric potential
        for (int cell_idx = 0; cell_idx < _electric_potential_cells.size();
             cell_idx++) {
            _electric_potential_cells[cell_idx] = 0.0;
            auto cell_center = _parameters->cell_index_to_cell_center(cell_idx);
            for (auto& charge : charges) {
                _electric_potential_cells[cell_idx] +=
                    charge.get_electric_potential(cell_center);
            }
            // noise
            double sign = rand() % 2 ? 1.0 : -1.0;
            _electric_potential_cells[cell_idx] *= 1.0 + sign * noise_percent * static_cast<double>(rand()) / RAND_MAX;
            _electric_potential_cells[cell_idx] =
                physics::clamp_potential(_electric_potential_cells[cell_idx]);
        }
    }

    std::vector<double> get_field_data() { return _electric_potential_cells; }

    double get_potential(const Eigen::Vector3d& position) {
        int cell_idx = _parameters->coords_to_cell_index(position);
        return _electric_potential_cells[cell_idx];
    }

    ~ElectricField() {}
};

}  // namespace physics
