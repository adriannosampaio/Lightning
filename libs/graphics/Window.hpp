#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>

#include <memory>
#include <utility>

namespace graphics {

namespace internal {

/**
 * @brief Functor to allow the SDL window destruction on the smart
 *  pointer
 *
 */
struct WindowDeleter {
    void operator()(SDL_Window* win) { SDL_DestroyWindow(win); }
};

/**
 * @brief Functor to allow the renderer's destruction on the smart
 *  pointer
 *
 */
struct RendererDeleter {
    void operator()(SDL_Renderer* rend) { SDL_DestroyRenderer(rend); }
};
}  // namespace internal

/**
 * @brief Class responsible for managing and destroying the
 *  SDL window
 */
class Window {
    /**
     * @brief Smart Pointer to an instance of an SDL window
     */
    std::unique_ptr<SDL_Window, internal::WindowDeleter> _window;
    /**
     * @brief Smart pointer to an SDL Renderer
     */
    std::unique_ptr<SDL_Renderer, internal::RendererDeleter> _renderer;
    /**
     * @brief Current stored dimensions of the window
     */
    std::array<unsigned, 2> _dimensions;

   public:
    /**
     * @brief Construct a new Window object
     *
     */
    Window();
    /**
     * @brief Construct a new Window object and allocate
     *   SDL objects.
     *
     * @param title Title displayed on the window
     * @param width width of the window in pixels
     * @param height height of the window in pixels
     */
    Window(const std::string& title, unsigned width, unsigned height);

    /**
     * @brief Destroy the Window object
     *
     */
    virtual ~Window();

    /**
     * @brief Get the renderer object
     *
     * @return std::shared_ptr<SDL_Renderer, internal::RendererDeleter>
     */
    SDL_Renderer* get_renderer() { return _renderer.get(); }
};

}  // namespace graphics