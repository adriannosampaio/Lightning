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

    Eigen::Vector3d _position;

   public:
    PathPoint() = delete;
    PathPoint(double x, double y, double z);
    void add_child(std::shared_ptr<PathPoint> path_point);
    Eigen::Vector3d& get_position() { return _position; }
    std::shared_ptr<PathPoint> get_child(unsigned idx) {
        return _children[idx];
    }
    std::vector<std::shared_ptr<PathPoint>> get_children() { return _children; }
    ~PathPoint();
};