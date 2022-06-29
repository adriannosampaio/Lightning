#pragma once
#include <memory>
#include <vector>

#include "Lightning.hpp"
#include "PathPoint.hpp"
#include "physics/ElectricCharge.hpp"
#include "physics/ElectricField.hpp"

struct DomainParameters {
    Eigen::Vector3d dimensions;
    Eigen::Vector3d cell_dimensions;
    Eigen::Vector3d cell_center_offset;
    Eigen::Vector3i number_of_cells;
    int number_of_cells_in_xy_plane;

    DomainParameters(const Eigen::Vector3d& dims, const Eigen::Vector3i& ncs) :
        dimensions(dims),
        number_of_cells(ncs),
        cell_dimensions(
            {dimensions[0] / number_of_cells[0],
             dimensions[1] / number_of_cells[1],
             dimensions[2] / number_of_cells[2]}),
        cell_center_offset(cell_dimensions / 2) {
        number_of_cells_in_xy_plane = number_of_cells[0] * number_of_cells[1];
    }
};

class Domain {
   private:
    constexpr static double COULUMBS_CONSTANT = 8.9875517923e9;
    DomainParameters _parameters;

    std::shared_ptr<physics::ElectricField> _field;
    std::vector<std::shared_ptr<Lightning>> _lightning_paths;
    std::vector<physics::ElectricCharge> _charges;
    std::vector<double> _electric_potential_cells;

   public:
    explicit Domain(DomainParameters parameters) :
        _parameters(parameters),
        _electric_potential_cells(
            std::vector<double>(parameters.number_of_cells.prod())) {}

    void set_parameters(DomainParameters parameters) {
        _parameters = parameters;
    }

    const DomainParameters& get_parameters() const { return _parameters; }

    void add_charge(const Eigen::Vector3d& position, double charge) {
        _charges.emplace_back(position, charge);
    }

    inline Eigen::Vector3i coords_to_cell(const Eigen::Vector3d& coords) {
        return {
            static_cast<int>(coords[0]) / _parameters.number_of_cells[0],
            static_cast<int>(coords[1]) / _parameters.number_of_cells[1],
            static_cast<int>(coords[2]) / _parameters.number_of_cells[2]};
    }

    inline Eigen::Vector3d cell_index_to_cell_center(int index) {
        int z_index = index / _parameters.number_of_cells_in_xy_plane;
        int y_index = (index % _parameters.number_of_cells_in_xy_plane) /
                      _parameters.number_of_cells[1];
        int x_index = index % _parameters.number_of_cells[1];
        return {
            x_index * _parameters.cell_dimensions[0] +
                _parameters.cell_center_offset[0],
            y_index * _parameters.cell_dimensions[1] +
                _parameters.cell_center_offset[1],
            z_index * _parameters.cell_dimensions[2] +
                _parameters.cell_center_offset[2]};
    }

    inline int coords_to_cell_index(const Eigen::Vector3d& coords) {
        auto cell_coords = this->coords_to_cell(coords);
        return cell_coords[2] * _parameters.number_of_cells[0] *
                   _parameters.number_of_cells[1] +
               cell_coords[1] * _parameters.number_of_cells[0] + cell_coords[0];
    }

    void generate_field() {
        // Calculate electric potential
        for (int cell_idx = 0; cell_idx < _electric_potential_cells.size();
             cell_idx++) {
            _electric_potential_cells[cell_idx] = 0.0;
            auto cell_center = this->cell_index_to_cell_center(cell_idx);
            for (auto& charge : _charges) {
                double distance_from_cell =
                    (cell_center - charge.position).norm();
                _electric_potential_cells[cell_idx] +=
                    (charge.charge / distance_from_cell);
            }
            _electric_potential_cells[cell_idx] *= COULUMBS_CONSTANT;
        }
    }

    std::shared_ptr<Lightning> generate_path(int seed = 0) {
        auto root = std::make_shared<PathPoint>(100, 0, 100);
        root->add_child(std::make_shared<PathPoint>(200, 0, 200));
        root->add_child(std::make_shared<PathPoint>(100, 0, 200));
        root->get_child(1)->add_child(std::make_shared<PathPoint>(150, 0, 300));
        root->get_child(1)->add_child(std::make_shared<PathPoint>(150, 0, 500));
        return std::make_shared<Lightning>(root);
    }

    void generate_paths(unsigned number_of_paths) {
        // static_assert(number_of_paths > 0);
        _lightning_paths.reserve(number_of_paths);
        for (unsigned i = 0; i < number_of_paths; i++) {
            _lightning_paths.push_back(generate_path());
        }
    }

    ~Domain() {}
};
