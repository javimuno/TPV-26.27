#pragma once
#include "MobileObject.h"

class Butterfly : public MobileObject
{
private:
	bool dead;
	float directionTimer;
	float nextDirectionChange;

	static constexpr float MIN_DIRECTION_DELAY = 1.0f;
	static constexpr float MAX_DIRECTION_DELAY = 3.0f;
	static constexpr float FALL_SPEED = 150.0f;

	int animationFrame;
	float animationTimer;

	static constexpr int NUM_ANIMATION_FRAMES = 10;
	static constexpr float ANIMATION_FRAME_TIME = 0.08f;

	float randomDirectionDelay();

protected: 
	bool shouldMove() const override;
	bool specificUpdate(float dt) override;

public: 
	Butterfly(Texture* text, const Point2D<float>& pos, const Vector2D<float>& dim, const Vector2D<float>& vel, Game* g);
	
	Butterfly(std::istream& is, Game* g);
	
	virtual ~Butterfly() = default;
	bool hit(const Rectangle& other) override;
	virtual bool canBeHit() const override { return true; };
	void render() const override;

	void save(std::ostream& os) const override;
};

