#pragma once

#include "MobileObject.h"

class Butterfly : public MobileObject
{

public:

	Butterfly(Game* game, const Rectangle& rect,
		const Vector2D <>& velocity, Texture* texture);

	void render() const override;
	bool update(float delta) override;


private:

	int color = 0;
	int col = 0;
	int dir = 0;


};

