#pragma once

#include "rectangle.h"
#include "texture.h"

#include "vector2D.h"

class Game;
class Rectangle;

class Bow
{
private:

	Rectangle* rect;
	Vector2D<float> velocity;
	Point2D<float> position;
	Texture* texture;
	
	bool isLoaded;
	int numArrows;

	Game* game;
public:

	Bow(Texture* text, const Point2D<float>& pos, const Vector2D<float>& vel, const int iniNumArrows, Game* game);
	~Bow();
	void render() const;
	bool update(float delta);
	void handleEvent(const SDL_Event& event);
};

