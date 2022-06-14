#ifndef _RENDERABLE_HPP_
#define _RENDERABLE_HPP_
#include <SDL.h>

#include <array>
#include <memory>

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

}  // namespace graphics
#endif  //_RENDERABLE_HPP_