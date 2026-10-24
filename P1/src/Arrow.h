#pragma once

#include "rectangle.h"
#include "texture.h"
#include "vector2D.h"

class Game;

class Arrow
{
private:

	Rectangle rect;
	Vector2D<> vel;
	Texture* textArrow;
	Game* game;
	int dimTip=20;
	

public:

	Arrow(const Rectangle& rect, const Vector2D<>& vel, Texture* textArrow,Game* game);
	void render() const;
	bool update(float delta);




};

