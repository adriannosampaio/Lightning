#pragma once

#include <Eigen/Dense>
#include <cmath>
#include <vector>

namespace physics {
/**
 * @brief Coulumb's constant for use in the electric equations
 */
constexpr double COULUMBS_CONSTANT = 8.9875517923e9;

/**
 * @brief Maximum allowed value for the electric potential.
 * This is limited to allow an easy way to visualize the potential
 * on pixels of an image
 */
constexpr double MAX_POTENTIAL = 1e3 * COULUMBS_CONSTANT;

/**
 * @brief Clamp a value limiting it's maximum and minimum values
 *
 * @param value the value to be processed
 * @param min_value minimum value accepted
 * @param max_value maximum value accepted
 * @return the result of the clamp operation
 */
inline double clamp(double value, double min_value, double max_value) {
    return std::max(min_value, std::min(max_value, value));
}

/**
 * @brief Function to clamp an electric potential using the
 *  already defined physics::MAX_POTENTIAL
 *
 * @param value the electric potential value to be clamped
 * @return the clamped value
 */

inline double clamp_potential(double value) {
    return clamp(value, -MAX_POTENTIAL, MAX_POTENTIAL);
}

constexpr double PI = 3.1415926535897932;

constexpr double SQRT_5 = 2.2360679775;

/** Golden angle in radians
 */
constexpr double PHI = PI * (3.0 - SQRT_5);

inline double degrees_to_radians(double degrees) { return degrees * PI / 180; }

/**
 * @brief Conversion of spherical coordinates (ρ, θ, φ) using the formulas
 *  shown below:
 *     - x = ρ sinφ cosθ
 *     - y = ρ sinφ sinθ
 *     - z = ρ cosφ
 *
 * @param radius
 * @param theta
 * @param phi
 */
inline Eigen::Vector3d spherical_to_cartesian(
    double radius, double theta, double phi) {
    Eigen::Vector3d result;
    result[0] = radius * std::sin(phi) * std::cos(theta);
    result[1] = radius * std::sin(phi) * std::sin(theta);
    result[2] = radius * std::cos(phi);
    return result;
}

/**
 * @briefw
 *
 * @param radius
 * @return Eigen::Vector3d
 */
inline Eigen::Vector3d generate_random_point_in_sphere(double radius = 1.0) {
    Eigen::Vector3d result;
    double theta = degrees_to_radians(std::rand() % 360);
    double phi = degrees_to_radians(std::rand() % 360);
    return spherical_to_cartesian(radius, theta, phi);
}

inline std::vector<Eigen::Vector3d> generate_fibonacci_sphere(
    const Eigen::Vector3d& center, double radius_, unsigned number_of_points) {
    std::vector<Eigen::Vector3d> points;
    points.reserve(number_of_points);
    for (size_t point = 0; point < number_of_points; point++) {
        double y = 1 - (point / static_cast<double>(number_of_points - 1)) * 2;
        double radius = std::sqrt(1 - y * y);  //#radius at y

        double theta = PHI * point;  // golden angle increment

        double x = std::cos(theta) * radius;
        double z = std::sin(theta) * radius;

        points.emplace_back(
            radius_ * x + center[0],
            radius_ * y + center[1],
            radius_ * z + center[2]);
    }
    return points;
}

inline double get_euclidean_distance(
    const Eigen::Vector3d& point_0, const Eigen::Vector3d& point_1) {
    return (point_0 - point_1).norm();
}

}  // namespace physics