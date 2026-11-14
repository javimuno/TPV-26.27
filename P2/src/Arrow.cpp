#include "Arrow.h"
#include "game.h"

bool Arrow::specificUpdate(float delta)
{
    float posLeft = rect->getTopLeftCorner().getX();

    float tipWidth = rect->getWidth() * 0.20f;

    Point2D<float> tipPosition(
        rect->getTopLeftCorner().getX() + rect->getWidth() - tipWidth,
        rect->getTopLeftCorner().getY()
    );

    Rectangle tip(tipPosition, tipWidth, rect->getHeight());

    game->checkCollision(tip);

    return posLeft < Game::WINDOW_WIDTH;
}

Arrow::Arrow(Texture* text, const Point2D<float>& pos, const Point2D<float>& dim, const Vector2D<float>& vel, Game* g)
	: MobileObject(text, pos, dim, vel, g) {}

bool Arrow::hit(const Rectangle& other)
{
	return false;
}
