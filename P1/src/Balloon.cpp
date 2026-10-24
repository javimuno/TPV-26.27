#include "Balloon.h"


Balloon::Balloon(const Rectangle& rect, const Vector2D<>& vel, Texture* textBalloon, int color) : rect(rect), vel(vel), textBalloon(textBalloon), color(color)
{

}

void Balloon::render() const
{
	textBalloon->renderFrame(rect.to_sdl(), color, 0);
}

//ture lo mantiene, false lo elimina
bool Balloon::update(float delta)
{
	Vector2D v1 = vel * delta;
	rect.move(v1);

	return((rect.getTopLeftCorner().getY() + rect.getHeight())>0);
}

bool Balloon::hit(const Rectangle& obj)
{
	if (rect.hasIntersection(obj)&&beenHit==false) {
		beenHit = true;
		std::cout << "globo golpeado" << std::endl;
		return true;
	}
	
	else
	{
		return false;
	}
}
