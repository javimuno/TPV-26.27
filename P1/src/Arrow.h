#pragma once

#include "vector2D.h"
//#include "rectangle.h"
class Rectangle;
class Texture;
class Game;
class Arrow
{
private:
	Texture* texture;
	Point2D<float> position;
	Vector2D<float> velocity;
	Rectangle* rect;
	
	Game* game;

	//Rectangle tip;
public:

	Arrow(Texture* text, const Point2D<float>& pos, const Vector2D<float>& vel, Game* g);
	~Arrow();
	void render() const;
	bool update(float delta);
};

