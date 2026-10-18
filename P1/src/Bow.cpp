#include "Bow.h"

Bow::Bow(const Rectangle& rect, const Vector2D<>& vel, Texture* textBow, int picture, const int numArrows, const bool armed) : rect(rect),
vel(vel),textBow(textBow),picture(picture), numArrows(numArrows), armed(armed)
{

}

void Bow::render()
{
	textBow->renderFrame(rect.to_sdl(), picture, 0);

}
