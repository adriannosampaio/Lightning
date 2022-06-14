#include "Graphics.hpp"

#include <SDL.h>

namespace graphics {

void initialize() { SDL_Init(SDL_INIT_EVERYTHING); }

void finalize() { SDL_Quit(); }

}  // namespace graphics