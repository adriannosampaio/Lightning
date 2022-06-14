#pragma once
#include <SDL.h>

#include <array>
#include <memory>

#define UNPACK(P) P[0], P[1]

namespace graphics {

class Renderable {
   protected:
    int _lifetime;

   public:
    Renderable(int lifetime);
    ~Renderable();
    void reduce_lifetime();
    virtual void render(SDL_Renderer* renderer) const = 0;
};

Renderable::Renderable(int lifetime = 0) : _lifetime(lifetime) {}

Renderable::~Renderable() {}

void Renderable::reduce_lifetime() {
    if (_lifetime) {
        _lifetime--;
    }
}

class Line : public Renderable {
   private:
    std::array<int, 2> _point_0;
    std::array<int, 2> _point_1;

   public:
    Line(
        const std::array<int, 2>& point_0,
        const std::array<int, 2>& point_1,
        unsigned lifetime = 0);
    ~Line();
    void render(SDL_Renderer* renderer) const override;
};

Line::Line(
    const std::array<int, 2>& point_0,
    const std::array<int, 2>& point_1,
    unsigned lifetime) :
    Renderable(lifetime), _point_0(point_0), _point_1(point_1) {}

Line::~Line() {}

void Line::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, UNPACK(_point_0), UNPACK(_point_1));
}

}  // namespace graphics
#undef UNPACK