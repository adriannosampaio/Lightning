#pragma once
#include <Eigen/Dense>
#include <memory>
#include <vector>

#include "Utils.hpp"

struct DomainParameters {
    Eigen::Vector3d dimensions;
    Eigen::Vector3i number_of_cells;
    Eigen::Vector3d cell_dimensions;
    Eigen::Vector3d cell_center_offset;
    double noise = 0.2;
    double max_cell_dimension;
    int number_of_cells_in_xy_plane;
    int new_points_per_leader = 30;
    int maximum_accepted_candidates = 3;
    double minimum_candidate_distance = 10;
    double lightning_segment_size;
    double accepted_distance_to_end_point;
    unsigned max_iterations = 5000;

    DomainParameters() {}

    DomainParameters(Eigen::Vector3d dims, Eigen::Vector3i ncs) :
        dimensions(dims),
        number_of_cells(ncs),
        cell_dimensions(
            {dimensions[0] / number_of_cells[0],
             dimensions[1] / number_of_cells[1],
             dimensions[2] / number_of_cells[2]}),
        max_cell_dimension(cell_dimensions.maxCoeff()),
        cell_center_offset(cell_dimensions / 2),
        minimum_candidate_distance(max_cell_dimension * 2),
        lightning_segment_size(max_cell_dimension),
        accepted_distance_to_end_point(2 * lightning_segment_size)

    {
        number_of_cells_in_xy_plane = number_of_cells[0] * number_of_cells[1];
    }

    inline Eigen::Vector3d cell_index_to_cell_center(int index) const {
        int z_index = index / number_of_cells_in_xy_plane;
        int y_index =
            (index % number_of_cells_in_xy_plane) / number_of_cells[0];
        int x_index = index % number_of_cells[0];
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
            static_cast<int>(std::floor(coords[0] / cell_dimensions[0])),
            static_cast<int>(std::floor(coords[1] / cell_dimensions[1])),
            static_cast<int>(std::floor(coords[2] / cell_dimensions[2]))};
    }

    inline Eigen::Vector3d clamp_coordinates_to_boundary(
        const Eigen::Vector3d& coords) const {
        return {
            core::clamp(coords[0], 0.0, dimensions[0]),
            core::clamp(coords[1], 0.0, dimensions[1]),
            core::clamp(coords[2], 0.0, dimensions[2])};
    }

    inline bool is_inside(const Eigen::Vector3d& coords) const {
        bool x = coords[0] > 0.0 && coords[0] < dimensions[0];
        bool y = coords[1] > 0.0 && coords[1] < dimensions[1];
        bool z = coords[2] > 0.0 && coords[2] < dimensions[2];
        return x && y && z;
    }
};
