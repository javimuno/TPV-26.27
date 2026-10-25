#include "Balloon.h"


Balloon::Balloon(const Rectangle& rect, const Vector2D<>& vel, Texture* textBalloon, int color) : rect(rect), vel(vel), textBalloon(textBalloon), color(color)
{

}

void Balloon::render() const
{
	textBalloon->renderFrame(rect.to_sdl(), color, col);
}

// Devuelve false al salir de pantalla o terminar la animacion de explosion.
bool Balloon::update(float delta)
{
	Vector2D v1 = vel * delta;
	rect.move(v1);

	// Acumula tiempo para mantener cada imagen de la explosion antes de avanzar.
	if (beenHit){
		acT += delta;
		if (acT > EXPLOSION_FRAME_TIME) {
			acT = 0.0f;
			col++;
		}
	}
	if (col > LAST_EXPLOSION_FRAME)return false; //fin animacion = eliminar

	return((rect.getTopLeftCorner().getY() + rect.getHeight())>0);
}

// El primer impacto inicia la explosion; los siguientes no vuelven a puntuar.
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
