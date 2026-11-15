#include "Bow.h"
#include "game.h"
#include "rectangle.h"

#include <iostream>

Bow::Bow(Texture* text, const Point2D<float>& pos, const Vector2D<float>& dim, Game* game) 
	: SceneObject(game, text, pos, dim),
    isLoaded(false)
{
}

bool Bow::hit(const Rectangle& other)
{
    return false;
}

void Bow::render() const
{
	if(isLoaded){
		texture->renderFrame(rect->to_sdl(), 0, 0);
	}
	else {
		texture->renderFrame(rect->to_sdl(), 0, 1);
	}
}

bool Bow::update(float delta)
{
	rect->move(velocity * delta);

	const float y = rect->getTopLeftCorner().getY();
	const float limitY = Game::WINDOW_HEIGHT - rect->getHeight();

	if (y < 0.0f) {
		rect->move({ 0.0f, -y });
	}
	else if (y > limitY) {
		rect->move({ 0.0f, limitY - y });
	}

	return true;
}


void Bow::handleEvent(const SDL_Event& event)
{
    // Cargar una flecha
    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_LEFT)
    {
        if (game->getArrows() > 0 && !isLoaded)
        {
            isLoaded = true;
        }
    }
    // Disparar la flecha
    else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_RIGHT)
    {
        if (isLoaded)
        {
           const float x = rect->getTopLeftCorner().getX();

            const float y = (rect->getTopLeftCorner().getY() + (rect->getHeight()) / 2.0f) - 6.0f;

            game->addArrow({ x, y }, { 190.0f, 0.0f });

            game->updateNumArrows();

            isLoaded = false;
        }
    }
    // Movimiento del arco
    else if (event.type == SDL_EVENT_KEY_DOWN)
    {
        if (event.key.key == SDLK_UP)
        {
            velocity = { 0.0f, -200.0f };
        }
        else if (event.key.key == SDLK_DOWN)
        {
            velocity = { 0.0f, 200.0f };
        }
    }
    // Detener el movimiento al soltar la tecla
    else if (event.type == SDL_EVENT_KEY_UP)
    {
        if (event.key.key == SDLK_UP && velocity.getY() < 0.0f)
        {
            velocity = { 0.0f, 0.0f };
        }
        else if (event.key.key == SDLK_DOWN && velocity.getY() > 0.0f)
        {
            velocity = { 0.0f, 0.0f };
        }
    }
}
