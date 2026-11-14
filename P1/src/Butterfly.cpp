#include "Butterfly.h"
#include "texture.h"
#include "game.h"

Butterfly::Butterfly(Game* game, const Rectangle& rect, const Vector2D<>& velocity, Texture* texture): MobileObject(game,rect,velocity,texture)
{}

void Butterfly::render() const
{
	getTexture()->renderFrame(getRect().to_sdl(), color, col);

}

bool Butterfly::update(float delta)
{

	MobileObject::update(delta);

	const float x = getRect().getTopLeftCorner().getX();
	const float maxX = Game::WINDOW_WIDTH - getRect().getWidth();

	if (x < 0.0f) {
		move(Vector2D<>(-x, 0.0f));
		reverseHorizontalDirection();
	}
	else if (x > maxX) {
		move(Vector2D<>(maxX - x, 0.0f));
		reverseHorizontalDirection();
	}
	return false;
}
