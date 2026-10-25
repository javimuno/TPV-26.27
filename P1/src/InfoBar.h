#pragma once

#include <SDL3/SDL.h>

class InfoBar {
private:
    SDL_Renderer* renderer;

public:
    InfoBar(SDL_Renderer* renderer);

    void render(int arrows) const;
};

