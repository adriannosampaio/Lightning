#include "PathPoint.hpp"

PathPoint::PathPoint(double x, double y, double z) :
    position(Eigen::Vector3d(x, y, z)) {}

PathPoint::PathPoint(const Eigen::Vector3d& vec) : position(vec) {}

void PathPoint::add_child(std::shared_ptr<PathPoint> path_point) {
    children.push_back(path_point);
}

PathPoint::~PathPoint() {}