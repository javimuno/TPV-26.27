#include "Bow.h"

Bow::Bow(const Rectangle& rect, const Vector2D<>& vel, Texture* textBow, int picture, const int numArrows, const bool armed) : rect(rect),
vel(vel),textBow(textBow),col(picture), numArrows(numArrows), armed(armed)
{

}

void Bow::render() const
{
	textBow->renderFrame(rect.to_sdl(), col, 0);

}

void Bow::update(float delta)
{
	Vector2D<> v1 = vel * delta;
	rect.move(v1);
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
		if (event.key.key == SDLK_UP&&vel.getY()<0) { // lo del &&vel.getY()<0 lo hace más suave, no hay un parón brusco
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
