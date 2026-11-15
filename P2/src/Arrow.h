#pragma once
#include "MobileObject.h"
#include "vector2D.h"
//#include "rectangle.h"
class Rectangle;
class Texture;
class Game;
class Arrow : public MobileObject
{
protected:
	bool specificUpdate(float delta) override;

public:

	Arrow(Texture* text, const Point2D<float>& pos, const Point2D<float>& dim, const Vector2D<float>& vel, Game* g);

	Arrow(std::istream& is, Game* g);

	virtual ~Arrow() = default;

	bool hit(const Rectangle& other) override;

	void save(std::ostream& os) const override;
};

