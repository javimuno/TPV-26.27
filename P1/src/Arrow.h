#pragma once

#include "rectangle.h"
#include "texture.h"
#include "vector2D.h"

class Game;

class Arrow
{
private:

	Rectangle rect;
	Vector2D<> vel;
	Texture* textArrow;
	Game* game;
	static constexpr float TIP_SIZE = 20.0f;

public:

	Arrow(const Rectangle& rect, const Vector2D<>& vel, Texture* textArrow,Game* game);
	void render() const;
	bool update(float delta);




};

