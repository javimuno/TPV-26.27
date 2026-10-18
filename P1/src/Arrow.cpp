#include "Arrow.h"
#include "game.h"

Arrow::Arrow(const Rectangle& rect, const Vector2D<>& vel, Texture* textArrow): rect(rect),vel(vel),textArrow(textArrow)
{

}

void Arrow::render() const
{
	textArrow->renderFrame(rect.to_sdl(), 0, 0);

}

bool Arrow::update(float delta)
{
	Vector2D<> v1 = vel * delta;
	rect.move(v1);

	return rect.getTopLeftCorner().getX()<Game::WINDOW_WIDTH;
}
