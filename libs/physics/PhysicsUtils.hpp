#pragma once

namespace physics {
constexpr double COULUMBS_CONSTANT = 8.9875517923e9;
constexpr double MAX_POTENTIAL = 1e3 * COULUMBS_CONSTANT;
double clamp(double value, double min_value, double max_value) {
    return std::max(min_value, std::min(max_value, value));
}
double clamp_potential(double value) {
    return clamp(value, -MAX_POTENTIAL, MAX_POTENTIAL);
}

}  // namespace physics
