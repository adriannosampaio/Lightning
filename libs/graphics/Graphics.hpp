#pragma once

#include <memory>
#include <vector>

#include "Renderable.hpp"
#include "Window.hpp"

namespace graphics {

void initialize();

void finalize();

class Renderer {
   private:
    std::shared_ptr<graphics::Window> _window;
    std::vector<std::shared_ptr<graphics::Renderable>> _renderable_objects;
    void render_objects() {
        for (auto object_ptr : _renderable_objects) {
            object_ptr->render(_window->get_renderer());
        }
    }

   public:
    explicit Renderer(std::shared_ptr<graphics::Window> window);
    void run();
    void add_renderable(std::shared_ptr<graphics::Renderable> object);

    ~Renderer();
};

Renderer::Renderer(std::shared_ptr<graphics::Window> window) :
    _window(window) {}

Renderer::~Renderer() {}

void Renderer::run() {
    bool finished = false;
    while (!finished) {
        SDL_Event event;

        // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    // handling of close button
                    finished = true;
                    break;
                }
            }
        }
        SDL_SetRenderDrawColor(_window->get_renderer(), 0, 0, 0, 255);
        SDL_RenderClear(_window->get_renderer());
        this->render_objects();
        SDL_RenderPresent(_window->get_renderer());
    }
}

void Renderer::add_renderable(std::shared_ptr<graphics::Renderable> object) {
    _renderable_objects.push_back(object);
}

}  // namespace graphics