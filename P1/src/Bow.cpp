#include "Bow.h"
#include "game.h"
#include "rectangle.h"

#include <iostream>

Bow::Bow(Texture* text, const Point2D<float>& pos, const Vector2D<float>& vel, const int iniNumArrows, Game* game) : 
	texture(text), position(pos), velocity(vel), numArrows(iniNumArrows), isLoaded(false), game(game)
{
	int w = float(texture->getFrameWidth());
	int h = float(texture->getFrameHeight());

	rect = new Rectangle(pos, w * Game::GAME_SCALE, h * Game::GAME_SCALE);
	std::cout << "Arrows: " << numArrows << std::endl;
}

Bow::~Bow()
{
    delete rect;
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
        if (numArrows > 0 && !isLoaded)
        {
            numArrows--;
            isLoaded = true;
        }
    }
    // Disparar la flecha
    else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_RIGHT)
    {
        if (isLoaded)
        {
            float wArrow = (float(game->getTexture(Game::ARROW)->getFrameWidth()) * Game::GAME_SCALE);
            int hArrow = (float(game->getTexture(Game::ARROW)->getFrameHeight()) * Game::GAME_SCALE)/2;

            const float x = rect->getTopLeftCorner().getX();

            const float y = (rect->getTopLeftCorner().getY() + (rect->getHeight()) / 2.0f) - 6.0f;

            game->addArrow({ x, y }, { 190.0f, 0.0f });

            game->updateNumArrows(numArrows);

            isLoaded = false;

            std::cout << "Arrows: " << numArrows << std::endl;
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
