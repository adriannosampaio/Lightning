#pragma once

#include <memory>
#include <vector>

#include "Renderable.hpp"
#include "Window.hpp"

namespace graphics {
class Renderer {
   private:
    /**
     * @brief Store the SDL window wrapper
     *
     */
    std::shared_ptr<graphics::Window> _window;
    /**
     * @brief store a list of objects to be rendered in the SDL window
     *
     */
    std::vector<std::shared_ptr<graphics::Renderable>> _renderable_objects;
    /**
     * @brief Iterate through the list of Renderable objects and render
     * them in the window
     *
     */
    void render_objects() {
        for (auto object_ptr : _renderable_objects) {
            object_ptr->render(_window->get_renderer());
        }
    }

   public:
    /**
     * @brief Construct a new Renderer object
     *
     * @param window
     */
    explicit Renderer(std::shared_ptr<graphics::Window> window);
    /**
     * @brief Start running the SDL window
     *
     */
    void run();
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
