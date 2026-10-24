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
	bool beenHit=false;
	int col=0;
	float acT = 0.0f;
	const float tExp=0.1f;



public:

	Balloon(const Rectangle& rect, const Vector2D<>& vel, Texture* textBalloon, int color);
	void render() const;
	bool update(float delta);
	bool hit(const Rectangle& obj);
};

