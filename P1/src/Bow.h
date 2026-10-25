#pragma once

#include "rectangle.h"
#include "texture.h"

#include "vector2D.h"
#include <SDL3/SDL_events.h>

class Game;

class Bow
{

private:
	static constexpr float MOVE_SPEED = 100.0f;
	static constexpr float ARROW_SPEED = 250.0f;
	static constexpr float ARROW_WIDTH = 80.0f;
	static constexpr float ARROW_HEIGHT = 20.0f;
	static constexpr float ARROW_OVERLAP = 20.0f;

	Rectangle rect;
	Vector2D<> vel;
	Texture* textBow;
	int numArrows;
	bool armed;

	Game* game;
	void handleShooting(const SDL_Event& event);

public:

	Bow(const Rectangle& rect, const Vector2D<>& vel,Texture* textBow, const int numArrows, const bool armed, Game* game);
	void render() const;
	bool update(float delta);
	void handleEvent(const SDL_Event& event);
	bool hasArrows() const;


};

