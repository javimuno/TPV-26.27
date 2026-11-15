#include "Balloon.h"
#include "texture.h"
#include "game.h"


Balloon::Balloon(Game* game,const Rectangle& rect, const Vector2D<>& vel, Texture* textBalloon, int color) : MobileObject(game,rect,vel,textBalloon),color(color)
{

}

void Balloon::render() const
{
	getTexture()->renderFrame(getRect().to_sdl(), color, col);
}
// Devuelve false al salir de pantalla o terminar la animacion de explosion.
bool Balloon::update(float delta)
{
	

	// Acumula tiempo para mantener cada imagen de la explosion antes de avanzar.
	if (beenHit){
		timeAccumulator += delta;
		if (timeAccumulator > EXPLOSION_FRAME_TIME) {
			timeAccumulator = 0.0f;
			col++;
		}
	}
	else
	{
		MobileObject::update(delta);
	}
	if (col > LAST_EXPLOSION_FRAME)return false; //fin animacion = eliminar

	return((getRect().getTopLeftCorner().getY() + getRect().getHeight())>0);
}

// El primer impacto inicia la explosion; los siguientes no vuelven a puntuar.
bool Balloon::hit(const Rectangle& obj)
{
	if (getRect().hasIntersection(obj) && beenHit == false) {
		beenHit = true;
		col = 1;
		getGame()->changeScore(BALLOON_POINTS);
		return true;
	}
	
	else
	{
		return false;
	}
}

void Balloon::save(std::ostream& out) const
{
	out << "balloon ";
	MobileObject::save(out);
	out << ' ' << color << ' '
		<< beenHit << ' '
		<< col << ' '
		<< timeAccumulator << '\n';

}

//reconstruccion de balloon
Balloon* Balloon::load(std::istream& in, Game* game)
{
	const Rectangle rect = readRectangle(in);
	const Vector2D<> velocity = readVelocity(in);

	int savedColor, savedCol;
	bool savedBeenHit;
	float savedTimeAccumulator;

	if (!(in >> savedColor >> savedBeenHit >> savedCol >> savedTimeAccumulator)) {
		throw GameError("Datos de globo no validos");
	}

	Balloon* balloon = new Balloon(game, rect, velocity, game->getTexture(Game::BALLOON), savedColor);

	balloon->beenHit = savedBeenHit;
	balloon->col = savedCol;
	balloon->timeAccumulator = savedTimeAccumulator;

	return balloon;
}
