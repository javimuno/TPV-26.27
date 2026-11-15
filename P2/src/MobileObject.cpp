#include "MobileObject.h"
#include "FileFormatError.h"

void MobileObject::move(float delta)
{
	rect->move(speed * delta);
}

MobileObject::MobileObject(Texture* text, const Point2D<float>& pos, const Vector2D<float>& dim, const Vector2D<float>& vel, Game* g)
	: SceneObject(g, text, pos, dim), speed(vel) {}

MobileObject::MobileObject(std::istream& is, Texture* tex, Game* g)
	: SceneObject(is, g, tex), speed()
{
	float speedX, speedY;
	//pasar lineno??
	if (!(is >> speedX >> speedY))
		throw FileFormatError(
			"saved.txt",
			0,
			"Error al leer la velocidad de MobileObject"
		);

	speed = Vector2D<float>(speedX, speedY);
}

bool MobileObject::update(float dt)
{
	if (shouldMove())
		move(dt);

	return specificUpdate(dt);
}

void MobileObject::save(std::ostream& os) const {
	SceneObject::save(os);

	os << speed.getX() << " "
		<< speed.getY() << " ";
}
