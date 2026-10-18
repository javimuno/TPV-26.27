#include "Bow.h"
#include "game.h"

Bow::Bow(const Rectangle& rect, const Vector2D<>& vel, Texture* textBow, int picture, const int numArrows, const bool armed) : rect(rect),
vel(vel),textBow(textBow),col(picture), numArrows(numArrows), armed(armed)
{

}

void Bow::render() const
{
	textBow->renderFrame(rect.to_sdl(),0 , col);

}

bool Bow::update(float delta)
{
	Vector2D<> v1 = vel * delta;
	rect.move(v1);

	//limites de altura
	float y = rect.getTopLeftCorner().getY();
	float maxY = Game::WINDOW_HEIGHT - rect.getHeight();

	if (y < 0) {
		rect.move(Vector2D<>(0.0f, -y));
	}
	else if (y > maxY) {
		rect.move(Vector2D<>(0.0f, maxY-y));
	}

	return true;
}

void Bow::handleEvent(const SDL_Event& event)
{
	//subir
	if (event.type == SDL_EVENT_KEY_DOWN) {
		if (event.key.key == SDLK_UP) {
			vel = Vector2D<>(0.0f, -100.0f);
		}
	}
	else if (event.type == SDL_EVENT_KEY_UP) {
		if (event.key.key == SDLK_UP&&vel.getY()<0) { //si pulsas el sentido contrario pasa a velocidad opuesta sin pasar a 0,0
			vel = Vector2D<>(0.0f, 0.0f);
		}
	}                                                 

	//bajar
	if (event.type == SDL_EVENT_KEY_DOWN) {
		if (event.key.key == SDLK_DOWN) {
			vel = Vector2D<>(0.0f, 100.0f);
		}
	}
	else if (event.type == SDL_EVENT_KEY_UP) {
		if (event.key.key == SDLK_DOWN&&vel.getY()>0) {
			vel = Vector2D<>(0.0f, 0.0f);
		}
	}
}
