#pragma once

#include "rectangle.h"
#include "texture.h"
#include "vector2D.h"

#include "MobileObject.h"

class Game;

class Arrow : public MobileObject
{
private:

	static constexpr float TIP_SIZE = 20.0f;
	static constexpr int BONUS_POINTS = 3;
	int balloonsHit;

public:

	Arrow(const Rectangle& rect, const Vector2D<>& vel, Texture* textArrow,Game* game);
	~Arrow()override;
	void render() const override;
	bool update(float delta) override;
	void save(std::ostream& out) const override;
	Arrow (std::istream& in, Game* game);
};

