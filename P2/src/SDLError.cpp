#include "SDLError.h"
#include <SDL3/SDL.h>

SDLError::SDLError(const std::string& message)
    : GameError(message + ": " + SDL_GetError()) {}
