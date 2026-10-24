#include "Balloon.h"
#include "vector2D.h"
#include "texture.h"
#include "rectangle.h"

Balloon::Balloon(Texture* tex, const Point2D<float>& position, const Vector2D<float>& velocity, int c) 
	: texture(tex), pos(position), vel(velocity), color(c)
{
	int w = float(texture->getFrameWidth());
	int h = float(texture->getFrameHeight());
	rect = new Rectangle(position, w, h);
}

Balloon::~Balloon()
{
}

bool Balloon::hit(const Rectangle& obj)
{
	return false;
}

bool Balloon::update(float delta)
{
	return false;
}

void Balloon::render() const {
	texture->renderFrame(rect->to_sdl(), color, 0);
}
