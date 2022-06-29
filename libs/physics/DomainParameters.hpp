#pragma once
#include <Eigen/Dense>
#include <memory>
#include <vector>

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

    inline Eigen::Vector3d cell_index_to_cell_center(int index) const {
        int z_index = index / number_of_cells_in_xy_plane;
        int y_index =
            (index % number_of_cells_in_xy_plane) / number_of_cells[1];
        int x_index = index % number_of_cells[1];
        return {
            x_index * cell_dimensions[0] + cell_center_offset[0],
            y_index * cell_dimensions[1] + cell_center_offset[1],
            z_index * cell_dimensions[2] + cell_center_offset[2]};
    }

    inline int coords_to_cell_index(const Eigen::Vector3d& coords) const {
        auto cell_coords = this->coords_to_cell(coords);
        return cell_coords[2] * number_of_cells[0] * number_of_cells[1] +
               cell_coords[1] * number_of_cells[0] + cell_coords[0];
    }

    inline Eigen::Vector3i coords_to_cell(const Eigen::Vector3d& coords) const {
        return {
            static_cast<int>(coords[0]) / number_of_cells[0],
            static_cast<int>(coords[1]) / number_of_cells[1],
            static_cast<int>(coords[2]) / number_of_cells[2]};
    }
};
