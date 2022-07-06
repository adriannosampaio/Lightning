#pragma once

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

}  // namespace physics
