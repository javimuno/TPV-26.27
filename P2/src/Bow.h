#pragma once
#include "SceneObject.h"
#include "rectangle.h"
#include "texture.h"

#include "vector2D.h"

class Game;
class Rectangle;

class Bow : public SceneObject
{
private:
	Vector2D<float> velocity = {0, 0};

	bool isLoaded;

public:

	Bow(Texture* text, const Point2D<float>& pos, const Vector2D<float>& dim, Game* game);
	bool hit(const Rectangle& other) override;
	void render() const override;
	bool update(float delta) override;
	void handleEvent(const SDL_Event& event);
};

