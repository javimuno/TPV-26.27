#pragma once
#include "SceneObject.h"
#include "GameObject.h"
#include <iosfwd>

class MobileObject : public SceneObject
{
protected:
	virtual bool shouldMove() const { return true; }
	virtual bool specificUpdate(float dt) = 0;

	Vector2D<float> speed;

private:
	void move(float delta);

public:
	MobileObject(Texture* text, const Point2D<float>& pos, const Vector2D<float>& dim, const Vector2D<float>& vel, Game* g);

	MobileObject(std::istream& is, Texture* tex, Game* g);

	virtual bool update(float dt) override;

	void save(std::ostream& os) const override;
};
