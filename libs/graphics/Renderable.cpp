#include "Renderable.hpp"

namespace graphics {

Renderable::Renderable(int lifetime = 0) : _lifetime(lifetime) {}

Renderable::~Renderable() {}

void Renderable::reduce_lifetime() {
    if (_lifetime) {
        _lifetime--;
    }
}

Line::Line(
    const std::array<int, 2>& point_0,
    const std::array<int, 2>& point_1,
    unsigned lifetime) :
    Renderable(lifetime), _point_0(point_0), _point_1(point_1) {}

Line::~Line() {}

void Line::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(
        renderer, _point_0[0], _point_0[1], _point_1[0], _point_1[1]);
}

}  // namespace graphics