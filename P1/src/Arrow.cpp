#include "Arrow.h"
#include "game.h"

Arrow::Arrow(const Rectangle& rect, const Vector2D<>& vel, Texture* textArrow, Game* game) : rect(rect), vel(vel), textArrow(textArrow), game(game)
{

}

void Arrow::render() const
{
	textArrow->renderFrame(rect.to_sdl(), 0, 0);

	//rectangulo punta

	float tipX = rect.getTopLeftCorner().getX() + rect.getWidth() - dimTip;
	float tipY = rect.getTopLeftCorner().getY();
	Rectangle tip({ tipX,tipY }, dimTip, dimTip);
	game->drawDebugRect(tip);
}

bool Arrow::update(float delta)
{
	
	
	Vector2D<> v1 = vel * delta;
	rect.move(v1);

	//rectangulo punta

	float tipX = rect.getTopLeftCorner().getX() + rect.getWidth() - dimTip;
	float tipY = rect.getTopLeftCorner().getY();
	Rectangle tip({ tipX,tipY }, dimTip, dimTip);

	//colision
	game->checkCollision(tip);

	return rect.getTopLeftCorner().getX()<Game::WINDOW_WIDTH;
}
