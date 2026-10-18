#pragma once

#include "rectangle.h"
#include "texture.h"
#include "vector2D.h"

class Arrow
{
private:

	Rectangle rect;
	Vector2D<> vel;
	Texture* textArrow;
	

public:

	Arrow(const Rectangle& rect, const Vector2D<>& vel, Texture* textArrow);
	void render() const;
	bool update(float delta);




};

