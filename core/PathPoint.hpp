#pragma once

#include <Eigen/Dense>
#include <memory>
#include <vector>

/**
 * @brief Class that represents a point in the lightning path.
 *  It contains information about its position in space, next points
 * in the sequence and other relevant info
 *
 */
struct PathPoint {
    /**
     * @brief next points in line to continue the path
     *
     */
    std::vector<std::shared_ptr<PathPoint>> children;
    std::shared_ptr<PathPoint> parent = nullptr;
    bool is_main_branch = false;
    double weight;
    unsigned level = 0;
    Eigen::Vector3d position;

    PathPoint() = delete;
    PathPoint(double x, double y, double z);
    explicit PathPoint(const Eigen::Vector3d& vec);
    void add_child(std::shared_ptr<PathPoint> path_point);
    const Eigen::Vector3d& get_position() { return position; }
    double get_weight() { return weight; }
    void set_weight(double weight) { weight = weight; }
    std::shared_ptr<PathPoint> get_parent() { return parent; }
    void set_parent(std::shared_ptr<PathPoint> parent) { parent = parent; }
    std::shared_ptr<PathPoint> get_child(unsigned idx) {
        return children[idx];
    }
    std::vector<std::shared_ptr<PathPoint>> get_children() { return children; }
    ~PathPoint();
};