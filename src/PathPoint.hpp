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
class PathPoint {
   private:
    /**
     * @brief next points in line to continue the path
     *
     */
    std::vector<std::shared_ptr<PathPoint>> _children;
    double _weight;
    Eigen::Vector3d _position;

   public:
    PathPoint() = delete;
    PathPoint(double x, double y, double z);
    explicit PathPoint(const Eigen::Vector3d& vec);
    void add_child(std::shared_ptr<PathPoint> path_point);
    const Eigen::Vector3d& get_position() { return _position; }
    double get_weight() { return _weight; }
    void set_weight(double weight) { _weight = weight; }
    std::shared_ptr<PathPoint> get_child(unsigned idx) {
        return _children[idx];
    }
    std::vector<std::shared_ptr<PathPoint>> get_children() { return _children; }
    ~PathPoint();
};