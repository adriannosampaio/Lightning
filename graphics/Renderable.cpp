#include "Renderable.hpp"

namespace graphics {

Renderable::Renderable(int lifetime = -1) : _lifetime(lifetime) {}

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

Pixel::Pixel(const std::array<int, 2>& position, unsigned lifetime) :
    Renderable(lifetime), _position(position) {}

Pixel::~Pixel() {}

void Pixel::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawPoint(renderer, _position[0], _position[1]);
}

void Field::render(SDL_Renderer* renderer) const {
    for (int i = 0; i < _dimensions[0]; i++) {
        for (int j = 0; j < _dimensions[1]; j++) {
            int idx = i + _dimensions[0] * j;
            bool is_negative = _data[idx] < 0;
            double data = std::fabs(_data[idx]) / _max_value;
            int res = static_cast<int>(data * 255);
            SDL_SetRenderDrawColor(
                renderer, is_negative ? res : 0, 0, is_negative ? 0 : res, 255);
            SDL_RenderDrawPoint(renderer, i, j);
        }
    }
}

}  // namespace graphics