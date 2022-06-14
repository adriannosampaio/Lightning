#define SDL_MAIN_HANDLED  // insert this
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>

#include "Window.hpp"

namespace graphics {

void initialize() { SDL_Init(SDL_INIT_EVERYTHING); }

void finalize() { SDL_Quit(); }

}  // namespace graphics