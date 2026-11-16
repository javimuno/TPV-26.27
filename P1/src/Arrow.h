#pragma once

#include "rectangle.h"
#include "texture.h"
#include "vector2D.h"

#include "MobileObject.h"

class Game;

class Arrow : public MobileObject
{
private:

	static constexpr float TIP_SIZE = 20.0f;
	static constexpr int BONUS_POINTS = 3;
	int balloonsHit;

public:

	Arrow(const Rectangle& rect, const Vector2D<>& vel, Texture* textArrow,Game* game);
	~Arrow()override; //descuenta flecha en vuelo
	void render() const override;
	bool update(float delta) override; //mueve la flecha, comprueba impactos y devuelve false al salri
	void save(std::ostream& out) const override; //guarda la flecha y los globos que ha explotado
	Arrow (std::istream& in, Game* game); //reconstruye la flecha del saved.txt
};

