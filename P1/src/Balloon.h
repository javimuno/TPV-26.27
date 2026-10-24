#pragma once

#include "vector2D.h"
//static constexpr float BALLOON_SCALE = 0.25f;
class Rectangle;
class Texture;

class Balloon
{

private:

	Rectangle* rect;
	Vector2D <float> velocity;
	Point2D<float> position;
	Texture* texture;
	int color;

	bool exploded = false;
	float animationTimer = 0.0f;
	int explosionFrame = 0;

public:

	Balloon(Texture* text, const Point2D<float>& pos, const Vector2D<float>& vel, int c);
	~Balloon();
	
	bool hit(const Rectangle& obj);

	bool update(float delta);
	void render() const;
};

