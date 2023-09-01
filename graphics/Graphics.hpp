#pragma once

#include <memory>
#include <vector>

#include "Renderer.hpp"
#include "Renderable.hpp"
#include "PathPoint.hpp"

struct DomainParameters;

namespace graphics {

void initialize();

std::unordered_map<unsigned, std::vector<std::shared_ptr<graphics::Line>>> get_lines(
        DomainParameters& params,
        std::shared_ptr<PathPoint> _root) ;

std::vector<std::shared_ptr<Line>> get_lines_simple(
        DomainParameters& params,
        std::shared_ptr<PathPoint> _root) ;

void finalize();

}  // namespace graphics