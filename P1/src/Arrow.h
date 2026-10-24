#pragma once

#include "vector2D.h"
//#include "rectangle.h"
class Rectangle;
class Texture;
class Game;
class Arrow
{
private:

	Rectangle* rect;
	Vector2D<float> velocity;
	Point2D<float> position;
	Texture* texture;

	//Rectangle tip;
	
	Game* game;
public:

	Arrow(Texture* text, const Point2D<float>& pos, const Vector2D<float>& vel, Game* g);
	~Arrow();
	void render() const;
	bool update(float delta);
};

