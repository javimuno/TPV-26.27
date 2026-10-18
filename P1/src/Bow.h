#pragma once

#include "rectangle.h"
#include "texture.h"

#include "vector2D.h"



class Bow
{

private:

	Rectangle rect;
	Vector2D<> vel;
	Texture* textBow;
	int col;
	int numArrows;
	bool armed;



public:

	Bow(const Rectangle& rect, const Vector2D<>& vel,Texture* textBow,int col, const int numArrows, const bool armed);
	void render() const;
	bool update(float delta);
	void handleEvent(const SDL_Event& event);


};

