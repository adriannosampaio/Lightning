#pragma once
#include <Eigen/Dense>

namespace graphics {
typedef Eigen::Vector3i color_t;

inline color_t WHITE = {255, 255, 255};
inline color_t BLACK = {0, 0, 0};
inline color_t RED = {255, 0, 0};
inline color_t GREEN = {0, 255, 0};
inline color_t BLUE = {0, 0, 255};

}  // namespace graphics
