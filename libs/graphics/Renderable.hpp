#ifndef _RENDERABLE_HPP_
#define _RENDERABLE_HPP_
#include <SDL.h>

#include <Eigen/Dense>
#include <array>
#include <memory>
#include <vector>

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

class Pixel : public Renderable {
   private:
    std::array<int, 2> _position;

   public:
    explicit Pixel(const std::array<int, 2>& position, unsigned lifetime = 0);
    ~Pixel();
    void render(SDL_Renderer* renderer) const override;
};

/**
 * @brief Allow for the rendering of a 2d physics field intensity, be it
 *  electric, magnetic, gravitaional. It assumes the data is organized as
 *  a linearized 2d matrix. Each data entry will be rendered as a pixel in
 *  the window
 *
 *  TODO: Allow for the grid to be adapted to the window resolution
 *
 */
class Field : public Renderable {
    /**
     * @brief 2D Dimensions of the grid in number of grid cells,
     *  i.e., pixels on each X and Y axis
     */
    std::array<int, 2> _dimensions;
    /**
     * @brief The double-precision float values to be converted into pixels.
     * Those values will be normalized in relation to graphics::Field::max_value
     * and graphics::Field::min_value
     *
     */
    std::vector<double> _data;
    /**
     * @brief A set of graphics::Pixel structures that define the field
     *  to be drawn
     */
    std::vector<Pixel> _pixels;
    /**
     * @brief Max possible value for the field. This will be mapped
     * as the maximum pixel intensity (i.e., 255 or white)
     */
    double _max_value;
    /**
     * @brief Min possible value for the field. This will be mapped
     * as the minimum pixel intensity (i.e., 0 or black)
     */
    double _min_value;

   public:
    Field(
        const std::array<int, 2>& dimensions,
        const std::vector<double>& data,
        double max_value = 255,
        double min_value = 0,
        int lifetime = -1) :
        Renderable(lifetime),
        _dimensions(dimensions),
        _data(data),
        _max_value(max_value),
        _min_value(min_value) {}
    ~Field() {}
    void render(SDL_Renderer* renderer) const override;
};

}  // namespace graphics
#endif  //_RENDERABLE_HPP_