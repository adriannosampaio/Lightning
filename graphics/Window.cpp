#include "Window.hpp"

namespace graphics {
Window::Window(const std::string& title, unsigned width, unsigned height) :
    _dimensions({width, height}),
    // creates a window
    _window(std::unique_ptr<SDL_Window, graphics::internal::WindowDeleter>(
        SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            0))),
    // creates a renderer to render our images
    _renderer(
        std::unique_ptr<SDL_Renderer, graphics::internal::RendererDeleter>(
            SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED))) {}

Window::~Window() {}

}  // namespace graphics