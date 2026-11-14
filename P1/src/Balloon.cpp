#include "Balloon.h"
#include "texture.h"


Balloon::Balloon(Game* game,const Rectangle& rect, const Vector2D<>& vel, Texture* textBalloon, int color) : MobileObject(game,rect,vel,textBalloon),color(color)
{

}

void Balloon::render() const
{
	getTexture()->renderFrame(getRect().to_sdl(), color, col);
}
// Devuelve false al salir de pantalla o terminar la animacion de explosion.
bool Balloon::update(float delta)
{
	

	// Acumula tiempo para mantener cada imagen de la explosion antes de avanzar.
	if (beenHit){
		timeAccumulator += delta;
		if (timeAccumulator > EXPLOSION_FRAME_TIME) {
			timeAccumulator = 0.0f;
			col++;
		}
	}
	else
	{
		MobileObject::update(delta);
	}
	if (col > LAST_EXPLOSION_FRAME)return false; //fin animacion = eliminar

	return((getRect().getTopLeftCorner().getY() + getRect().getHeight())>0);
}

// El primer imptimeAccumulatoro inicia la explosion; los siguientes no vuelven a puntuar.
bool Balloon::hit(const Rectangle& obj)
{
	if (getRect().hasIntersection(obj) && beenHit == false) {
		beenHit = true;
		col = 1;
		//std::cout << "globo golpeado" << std::endl;
		return true;
	}
	
	else
	{
		return false;
	}
}
