#include "InfoBar.h"

#include <string>

InfoBar::InfoBar(SDL_Renderer* renderer)
    : renderer(renderer)
{
}

void InfoBar::render(int arrows) const
{
    // Dibujar el fondo de la barra
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);

    SDL_FRect bar = { 0.0f, 0.0f, 200.0f, 35.0f };
    SDL_RenderFillRect(renderer, &bar);

    // Dibujar el numero de flechas restantes
    std::string text = "Flechas restantes: " + std::to_string(arrows);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(renderer, 10.0f, 12.0f, text.c_str());
}
