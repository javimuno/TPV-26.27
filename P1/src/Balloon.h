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
	static constexpr float EXPLOSION_FRAME_TIME = 0.1f;
	static constexpr int LAST_EXPLOSION_FRAME = 5;



public:

	Balloon(const Rectangle& rect, const Vector2D<>& vel, Texture* textBalloon, int color);
	void render() const;
	bool update(float delta);
	bool hit(const Rectangle& obj);
};

