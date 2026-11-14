#pragma once

#include "MobileObject.h"

class Butterfly : public MobileObject
{

public:

	Butterfly(Game* game, const Rectangle& rect,
		const Vector2D <>& velocity, Texture* texture);

	void render() const override;
	bool update(float delta) override;
	bool hit(const Rectangle& tip)override;


private:

	
	int col = 0;
	int dir = 0;
	static constexpr float DIRECTION_CHANGE_INTERVAL = 1.5f;
	float directionTimer;
	static constexpr float FALL_SPEED = 100.0f;
	static constexpr int HIT_PENALTY = -10;
	static constexpr int FLAP_FRAMES = 10;
	static constexpr float FLAP_FRAME_TIME = 0.06f;
	static constexpr int LEFT_FLAP_ROW = 0;
	static constexpr int RIGHT_FLAP_ROW = 1;
	int animationRow;

	float flapTimer;
	bool falling;

	void updateAnimation(float delta);
	int animBack;


};

