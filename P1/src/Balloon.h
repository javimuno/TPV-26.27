#pragma once

#include "rectangle.h"
#include "texture.h"


class Balloon
{

private:

	Rectangle rect;
	Vector2D <> vel;
	Texture* textBalloon;
	int color;


public:

	Balloon(const Rectangle& rect, const Vector2D<>& vel, Texture* textBalloon, int color);
	void render() const;
	bool update(float delta);
};

