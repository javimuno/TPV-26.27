#include "Arrow.h"
#include "game.h"

Arrow::Arrow(const Rectangle& rect, const Vector2D<>& vel, Texture* textArrow, Game* game) : rect(rect), vel(vel), textArrow(textArrow), game(game)
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

	//rectangulo punta

	float tipX = rect.getTopLeftCorner().getX() + rect.getWidth() - TIP_SIZE;
	float tipY = rect.getTopLeftCorner().getY();
	Rectangle tip({ tipX,tipY }, TIP_SIZE, TIP_SIZE);

	//colision
	game->checkCollision(tip);

	return rect.getTopLeftCorner().getX()<Game::WINDOW_WIDTH;
}
