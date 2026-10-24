#include "Balloon.h"


Balloon::Balloon(const Rectangle& rect, const Vector2D<>& vel, Texture* textBalloon, int color) : rect(rect), vel(vel), textBalloon(textBalloon), color(color)
{

}

void Balloon::render() const
{
	textBalloon->renderFrame(rect.to_sdl(), color, col);
}

//ture lo mantiene, false lo elimina
bool Balloon::update(float delta)
{
	Vector2D v1 = vel * delta;
	rect.move(v1);

	//acumulador animacion
	acT += tExp;

	if (beenHit){
		acT += delta;
		if (acT > tExp) {
			acT = 0.0f;
			col++;
		}
	}
	if (col > 5)return false; //fin animacion = eliminar

	return((rect.getTopLeftCorner().getY() + rect.getHeight())>0);
}

bool Balloon::hit(const Rectangle& obj)
{
	if (rect.hasIntersection(obj)&&beenHit==false) {
		beenHit = true;
		col = 1;
		std::cout << "globo golpeado" << std::endl;
		return true;
	}
	
	else
	{
		return false;
	}
}
