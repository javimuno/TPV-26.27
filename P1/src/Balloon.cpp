#include "Balloon.h"


Balloon::Balloon(const Rectangle& rect, const Vector2D<>& vel, Texture* textBalloon, int color) : rect(rect), vel(vel), textBalloon(textBalloon), color(color)
{

}

void Balloon::render() const
{
	textBalloon->renderFrame(rect.to_sdl(), color, 0);
}
