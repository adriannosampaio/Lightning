#pragma once

#include <memory>
#include <vector>

#include "Renderable.hpp"

namespace graphics {
class Renderer {
   private:
    /**
     * @brief store a list of objects to be rendered in the SDL window
     *
     */
    std::vector<std::shared_ptr<graphics::Renderable>> _renderable_objects;

    /**
     * @brief Iterate through the list of Renderable objects and render
     * them in the specified SDL_Renderer object
     *
     */
    void render_objects(SDL_Renderer* renderer) {
        for (auto object_ptr : _renderable_objects) {
            object_ptr->render(renderer);
        }
    }

   public:
    /**
     * @brief Construct a new Renderer object
     *
     * @param window
     */
    explicit Renderer();

    /**
     * @brief Start running the SDL window
     *
     */
    void run(const std::string& title, unsigned width, unsigned height);

    /**
     * @brief Saves an image instead of rendering it into a window
     *
     * @param filename
     */
    void generate_image(
        const std::string& filename, unsigned width, unsigned height);

    /**
     * @brief Add a new Renderable object to the list of objects
     * to be rendered
     *
     * @param object
     */
    void add_renderable(std::shared_ptr<graphics::Renderable> object);
    /**
     * @brief Destroy the Renderer object
     *
     */
    ~Renderer();
};

}  // namespace graphics
