#pragma once

#include "rectangle.h"
#include "texture.h"


class Bow
{

private:

	Rectangle rect;
	Vector2D<> vel;
	Texture* textBow;
	int picture;
	int numArrows;
	bool armed;



public:

	Bow(const Rectangle& rect, const Vector2D<>& vel,Texture* textBow,int picture, const int numArrows, const bool armed);
	void render();
	bool update(float delta);



};

