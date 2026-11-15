#pragma once
#include "MobileObject.h"
#include "vector2D.h"
#include <iosfwd>

class Rectangle;
class Texture;

class Balloon : public MobileObject
{

private:
	int color;
	int points;
	
	bool exploded = false;
	float animationTimer = 0.0f;
	int explosionFrame = 0;

public:

	Balloon(Texture* text, const Point2D<float>& pos, const Point2D<float>& dim, const Vector2D<float>& vel, Game* g, int c, int p);
	
	Balloon(std::istream& is, Game* g, int points);
	
	virtual ~Balloon() = default;
	
	bool hit(const Rectangle& other) override;

	void render() const override;

	virtual bool canBeHit() const override { return true; };

	void save(std::ostream& os) const override;

protected: 
	bool specificUpdate(float delta) override;
};

