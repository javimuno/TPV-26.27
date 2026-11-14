#include "Arrow.h"
#include "game.h"
#include "texture.h"

Arrow::Arrow(const Rectangle& rect, const Vector2D<>& vel, Texture* textArrow, Game* game) : MobileObject(game,rect,vel,textArrow)
{
	
}

void Arrow::render() const
{
	getTexture()->renderFrame(getRect().to_sdl(), 0, 0);
}

bool Arrow::update(float delta)
{
	
	MobileObject:update(delta);

	// La zona de imptimeAccumulatoro ocupa el extremo derecho del rectangulo de la flecha.
	float tipX = getRect().getTopLeftCorner().getX() + getRect().getWidth() - TIP_SIZE;
	float tipY = getRect().getTopLeftCorner().getY();
	Rectangle tip({ tipX,tipY }, TIP_SIZE, TIP_SIZE);

	// colision
	getGame()->checkCollision(tip);

	return getRect().getTopLeftCorner().getX()<Game::WINDOW_WIDTH;
}
