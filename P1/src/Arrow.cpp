#include "Arrow.h"
#include "game.h"
#include "texture.h"

Arrow::Arrow(const Rectangle& rect, const Vector2D<>& vel, Texture* textArrow, Game* game) : MobileObject(game,rect,vel,textArrow), balloonsHit(0)
{
	getGame()->arrowCreated();
}

Arrow::~Arrow()
{
	getGame()->arrowDestroyed();
}

void Arrow::render() const
{
	getTexture()->renderFrame(getRect().to_sdl(), 0, 0);
}

bool Arrow::update(float delta)
{
	
	MobileObject::update(delta);

	// La zona de impacto ocupa el extremo derecho del rectangulo de la flecha.
	float tipX = getRect().getTopLeftCorner().getX() + getRect().getWidth() - TIP_SIZE;
	float tipY = getRect().getTopLeftCorner().getY();
	Rectangle tip({ tipX,tipY }, TIP_SIZE, TIP_SIZE);

	// colision y bonus-> cuenta los globos anteriores y luego suma el actual
	const int pointsFromHit= getGame()->checkCollision(tip);

	if (pointsFromHit > 0) {
		const int bonus = BONUS_POINTS * balloonsHit;

		if (bonus > 0) {
			getGame()->changeScore(bonus);
		}
		++balloonsHit;
	}

	return getRect().getTopLeftCorner().getX()<Game::WINDOW_WIDTH;
}

void Arrow::save(std::ostream& out) const  
{
	out << "arrow ";
	MobileObject::save(out);
	out << ' ' << balloonsHit << '\n';
}

//reconstruccion de la flecha nuevo
Arrow::Arrow(std::istream& in, Game* game)
	: MobileObject(game, in, game->getTexture(Game::ARROW)),
	balloonsHit(0)
{
	if (!(in >> balloonsHit) || balloonsHit < 0) {
		throw GameError("Contador de globos no valido");
	}

	getGame()->arrowCreated();
}
