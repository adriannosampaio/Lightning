#include "Renderer.hpp"

#include <SDL_image.h>
#include <string>
#include <memory>
#include <string>

#include "Window.hpp"
namespace graphics {

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::run(const std::string& title, unsigned width, unsigned height) {
    // Create the window to be used
    auto window = std::make_shared<graphics::Window>(title, width, height);
    auto renderer = window->get_renderer();
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
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        this->render_objects(renderer);
        SDL_RenderPresent(renderer);
    }
}

void Renderer::generate_image(
    const std::string& filename, unsigned width, unsigned height) {
    // Allocating a new surface canvas
    SDL_Surface* surface =
        SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    // Creating a software renderer to avoid creating a window
    SDL_Renderer* renderer = SDL_CreateSoftwareRenderer(surface);
    // Cleaning the canvas
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // Rendering objects
    this->render_objects(renderer);
    // Save the actual image
    IMG_SavePNG(surface, filename.c_str());
    // Free the new surface
    SDL_FreeSurface(surface);
    // DEstroy the software renderer
    SDL_DestroyRenderer(renderer);
}

void Renderer::add_renderable(std::shared_ptr<graphics::Renderable> object) {
    _renderable_objects.push_back(object);
}

}  // namespace graphics