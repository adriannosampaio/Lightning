#pragma once
#include <array>
#include <memory>
#include <queue>
#include <vector>

#include "PathPoint.hpp"
#include "graphics/Renderable.hpp"
class Lightning {
   private:
    std::shared_ptr<PathPoint> _root;

   public:
    explicit Lightning(std::shared_ptr<PathPoint> root) : _root(root) {}
    std::shared_ptr<PathPoint> get_root() { return _root; }
    std::vector<std::shared_ptr<graphics::Line>> get_lines() {
        std::vector<std::shared_ptr<graphics::Line>> result;
        std::queue<std::shared_ptr<PathPoint>> queue;
        queue.push(_root);
        while (!queue.empty()) {
            auto current = queue.front();
            auto& current_pos = current->get_position();
            queue.pop();
            for (auto child : current->get_children()) {
                queue.push(child);
                auto& child_pos = child->get_position();
                result.push_back(std::make_shared<graphics::Line>(
                    std::array<int, 2>{
                        static_cast<int>(current_pos[0]),
                        static_cast<int>(current_pos[2])},
                    std::array<int, 2>{
                        static_cast<int>(child_pos[0]),
                        static_cast<int>(child_pos[2])}));
            }
        }
        return result;
    }
    ~Lightning() {}
};
