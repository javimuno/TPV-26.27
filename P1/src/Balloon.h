#pragma once

#include "MobileObject.h"

class Balloon : public MobileObject
{

private:

	
	int color;
	bool beenHit=false;
	int col=0;
	float timeAccumulator = 0.0f;
	static constexpr float EXPLOSION_FRAME_TIME = 0.1f;
	static constexpr int LAST_EXPLOSION_FRAME = 5;
	static constexpr int BALLOON_POINTS = 10;



public:

	Balloon(Game* game,const Rectangle& rect, const Vector2D<>& vel, Texture* textBalloon, int color);
	void render() const override;
	bool update(float delta) override; //devuelve false al salir por encima o al explotar	
	void save(std::ostream& out)const override; //guarda el estado del globo incluida la explosion
	Balloon(std::istream& in, Game* game); //reconstruye el globo desde saved.txt
	bool hit(const Rectangle& obj) override; //el primer impacto inicia explosiono y suma puntos (hit inicia y update anima, game lo elimina con update false
};
