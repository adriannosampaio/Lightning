#include <queue>

#include <SDL.h>

#include "Graphics.hpp"

#include "DomainParameters.hpp"

namespace graphics {

void initialize() { SDL_Init(SDL_INIT_EVERYTHING); }

std::vector<std::shared_ptr<graphics::Line>> get_lines(
        const std::shared_ptr<DomainParameters> params,
        std::shared_ptr<PathPoint> _root) {
    std::vector<std::shared_ptr<graphics::Line>> result;
    std::queue<std::shared_ptr<PathPoint>> queue;
    queue.push(_root);
    while (!queue.empty()) {
        auto current = queue.front();
        auto current_pos = params->coords_to_cell(current->get_position());
        queue.pop();
        for (auto child : current->get_children()) {
            queue.push(child);
            auto child_pos = params->coords_to_cell(child->get_position());
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

void finalize() { SDL_Quit(); }

}  // namespace graphics