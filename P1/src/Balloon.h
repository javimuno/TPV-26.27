#pragma once
#include "vector2D.h"

class Rectangle;
class Texture;
class Balloon
{
private:
	Rectangle* rect;
	Vector2D<float> vel;
	Point2D<float> pos;
	int color;
	Texture* texture;

public: 
	Balloon(Texture* tex, const Point2D<float>& position, const Vector2D<float>& velocity, int c);
	~Balloon();
	bool hit(const Rectangle& obj);
	void render() const;
	bool update(float delta);
};

