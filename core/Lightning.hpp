#pragma once
#include <array>
#include <memory>
#include <queue>
#include <vector>

#include "PathPoint.hpp"
#include "DomainParameters.hpp"

class Lightning {
   private:
    std::shared_ptr<PathPoint> _root;

   public:
    int max_level;
    explicit Lightning(std::shared_ptr<PathPoint> root) : _root(root) {}
    std::shared_ptr<PathPoint> get_root() { return _root; }

    ~Lightning() {}
};
