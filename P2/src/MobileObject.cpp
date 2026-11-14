#include "MobileObject.h"

void MobileObject::move(float delta)
{
	rect->move(speed * delta);
}

MobileObject::MobileObject(Texture* text, const Point2D<float>& pos, const Vector2D<float>& dim, const Vector2D<float>& vel, Game* g)
	: SceneObject(g, text, pos, dim), speed(vel) {}

bool MobileObject::update(float dt)
{
	if (shouldMove())
		move(dt);

	return specificUpdate(dt);
}
