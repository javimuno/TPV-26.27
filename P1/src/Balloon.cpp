#include "Balloon.h"
#include "vector2D.h"
#include "game.h"
#include "texture.h"

Balloon::Balloon(Texture* text, const Point2D<float>& pos, const Vector2D<float>& vel, int c)
	: texture(text), position(pos), velocity(vel), color(c)
{
	int w = float(texture->getFrameWidth());
	int h = float(texture->getFrameHeight());
	rect = new Rectangle(pos, w * Game::GAME_SCALE, h * Game::GAME_SCALE);
}

Balloon::~Balloon()
{
	delete rect;
}

bool Balloon::update(float delta)
{

	if (exploded)
	{
		animationTimer += delta;

		if (animationTimer >= 0.02f)
		{
			animationTimer -= 0.02f;
			explosionFrame++;

			if (explosionFrame > 6)
				return false;
		}

		return true;
	}

	rect->move(velocity * delta);

	float bottomY = rect->getTopLeftCorner().getY() + rect->getHeight();
	
	return bottomY > 0.0f;
}

void Balloon::render() const
{
	int frame = exploded ? explosionFrame : 0;
	texture->renderFrame(rect->to_sdl(), color, frame);
}

bool Balloon::hit(const Rectangle& obj)
{
	if (rect->hasIntersection(obj) && !exploded)
	{
		// Marcar el globo como explotado.
		explosionFrame = 0;
		animationTimer = 0.0f;
		exploded = true;
		return true;
	}

	return false;
}
