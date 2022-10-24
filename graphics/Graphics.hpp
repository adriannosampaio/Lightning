#pragma once

#include <memory>
#include <vector>

#include "Renderer.hpp"
#include "Renderable.hpp"
#include "PathPoint.hpp"

struct DomainParameters;

namespace graphics {

void initialize();

std::vector<std::shared_ptr<Line>> get_lines(
        const std::shared_ptr<DomainParameters> params,
        std::shared_ptr<PathPoint> _root) ;

void finalize();

}  // namespace graphics