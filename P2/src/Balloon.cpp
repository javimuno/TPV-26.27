#include "Balloon.h"
#include "vector2D.h"
#include "game.h"
#include "texture.h"

Balloon::Balloon(Texture* text, const Point2D<float>& pos, const Point2D<float>& dim, const Vector2D<float>& vel, Game* g, int c)
	: MobileObject(text, pos, dim, vel, g), color(c) {}

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

bool Balloon::specificUpdate(float delta)
{
	if (exploded)
	{
		animationTimer += delta;

		if (animationTimer >= 0.02f)
		{
			animationTimer -= 0.02f;
			explosionFrame++;

			if (explosionFrame > Game::NUM_FRAMES_BALLONS)
				return false;
		}

		return true;
	}

	float bottomY = rect->getTopLeftCorner().getY() + rect->getHeight();

	return bottomY > 0.0f;
}
