#pragma once

#include "rectangle.h"
#include "texture.h"
#include "SceneObject.h"

#include "vector2D.h"
#include <SDL3/SDL_events.h>


class Game;

class Bow : public SceneObject
{

private:
	static constexpr float MOVE_SPEED = 250.0f;
	static constexpr float ARROW_SPEED = 400.0f;
	static constexpr float ARROW_WIDTH = 100.0f;
	static constexpr float ARROW_HEIGHT = 20.0f;
	static constexpr float ARROW_OVERLAP = 20.0f;

	
	Vector2D<> vel;	
	int numArrows;
	bool armed;

	void handleShooting(const SDL_Event& event);

public:

	Bow(const Rectangle& rect, const Vector2D<>& vel,Texture* textBow, const int numArrows, const bool armed, Game* game);
	void render() const override;
	bool update(float delta) override;
	void handleEvent(const SDL_Event& event);
	bool hasArrows() const;
	void save(std::ostream& out) const override;
	Bow(std::istream& in, Game* game);
};

