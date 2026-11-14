#pragma once
#include "MobileObject.h"
#include "vector2D.h"

class Rectangle;
class Texture;

class Balloon : public MobileObject
{

private:
	int color;
	
	bool exploded = false;
	float animationTimer = 0.0f;
	int explosionFrame = 0;

public:

	Balloon(Texture* text, const Point2D<float>& pos, const Point2D<float>& dim, const Vector2D<float>& vel, Game* g, int c);
	virtual ~Balloon() = default;
	
	bool hit(const Rectangle& other) override;

	void render() const override;

	virtual bool canBeHit() const override { return true; };

protected: 
	bool specificUpdate(float delta) override;
};

