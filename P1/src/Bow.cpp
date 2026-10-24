#include "Bow.h"
#include "game.h"

#include <iostream>

Bow::Bow(const Rectangle& rect, const Vector2D<>& vel, Texture* textBow, int picture, const int numArrows, const bool armed, Game* game) : rect(rect),
vel(vel),textBow(textBow),col(picture), numArrows(numArrows), armed(armed), game(game)
{
	std::cout << "Flechas restantes: " << numArrows << std::endl;
}

void Bow::render() const
{
	int column;
	if(armed){
		column = 0;
	}
	else {
		column = 1;
	}
	textBow->renderFrame(rect.to_sdl(),0 , column);

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

	//localizacion del arco

	float x = rect.getTopLeftCorner().getX() + rect.getWidth()-20.0f;
	float y = rect.getTopLeftCorner().getY() + (rect.getHeight()-20.0f)/2;
	
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

	//cargar flecha

	if (event.type == SDL_EVENT_KEY_DOWN) {
		if (event.key.key == SDLK_LEFT && !armed && numArrows > 0) {
			armed = true;
			numArrows--;
			
		}
	}

	//soltar flecha

	if (event.type == SDL_EVENT_KEY_DOWN) {
		if (event.key.key == SDLK_RIGHT && armed) {
			game->addArrow(Rectangle({ x,y}, 80.0f, 20.0f), Vector2D<>(250, 0));
			armed = false;
			std::cout << "Flechas restantes: " << numArrows << std::endl;
		}
	}

}

bool Bow::hasArrows() const
{
	return (numArrows > 0 || armed );
	
}
