#include "PathPoint.hpp"

PathPoint::PathPoint(double x, double y, double z) :
    _position(Eigen::Vector3d(x, y, z)) {}

void PathPoint::add_child(std::shared_ptr<PathPoint> path_point) {
    _children.push_back(path_point);
}

PathPoint::~PathPoint() {}
