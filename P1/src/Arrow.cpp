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

	// colision y BONUS
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
	out << "arrow: ";
	MobileObject::save(out);
	out << ' ' << balloonsHit << '\n';
}
