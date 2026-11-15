#include "Bow.h"
#include "game.h"
#include "texture.h"

#include <iostream>


// Reserva una flecha al cargar y crea el proyectil al disparar.
void Bow::handleShooting(const SDL_Event& event)
{
	Game* game = getGame();
	//localizacion del arco

	float x = getRect().getTopLeftCorner().getX() + getRect().getWidth() - ARROW_OVERLAP;
	float y = getRect().getTopLeftCorner().getY() + (getRect().getHeight() - ARROW_HEIGHT) / 2.0f;

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
			
			game->addObject(new Arrow(Rectangle({ x,y }, ARROW_WIDTH, ARROW_HEIGHT), Vector2D<>(ARROW_SPEED, 0), game->getTexture(Game::ARROW), game));

			armed = false;
			std::cout << "Flechas restantes: " << numArrows << std::endl;

		}
	}
}

Bow::Bow(const Rectangle& rect, const Vector2D<>& vel, Texture* textBow, const int numArrows, const bool armed, Game* game) :SceneObject(game,rect,textBow),vel(vel),numArrows(numArrows),armed(armed)
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
	getTexture()->renderFrame(getRect().to_sdl(), 0, column);

}

bool Bow::update(float delta)
{
	move(vel * delta);

	//limites de altura
	float y = getRect().getTopLeftCorner().getY();
	float maxY = Game::WINDOW_HEIGHT - getRect().getHeight();

	if (y < 0) {
		move(Vector2D<>(0.0f, -y));
	}
	else if (y > maxY) {
		move(Vector2D<>(0.0f, maxY-y));
	}

	return true;
}

void Bow::handleEvent(const SDL_Event& event)
{

	//subir
	if (event.type == SDL_EVENT_KEY_DOWN) {
		if (event.key.key == SDLK_UP) {
			vel = Vector2D<>(0.0f, -MOVE_SPEED);
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
			vel = Vector2D<>(0.0f, MOVE_SPEED);
		}
	}
	else if (event.type == SDL_EVENT_KEY_UP) {
		if (event.key.key == SDLK_DOWN&&vel.getY()>0) {
			vel = Vector2D<>(0.0f, 0.0f);
		}
	}
	//manejo del armado y lanzamiento de la flecha 
	handleShooting(event);

}

bool Bow::hasArrows() const
{
	return (numArrows > 0 || armed );
	
}

void Bow::save(std::ostream& out) const
{
	out << "bow ";
	SceneObject::save(out);

	out << ' ' << vel.getX() << ' '
		<< vel.getY() << ' '
		<< numArrows << ' '
		<< armed<<'\n';

}

Bow::Bow(std::istream& in, Game* game)
	: SceneObject(game, in, game->getTexture(Game::BOW)),
	vel{}, numArrows(0), armed(false)
{
	float vx, vy;
	if (!(in >> vx >> vy >> numArrows >> armed) || numArrows < 0) {
		throw GameError("Parametros del arco invalidos");
	}

	vel = Vector2D<>(vx, vy);
}
