#include "SceneObject.h"

SceneObject::SceneObject(Game* game, Texture* tex, const Point2D<float>& pos, const Vector2D<float>& dim)
	: GameObject(game), texture(tex), position(pos), dimension(dim)
{
	int w = float(texture->getFrameWidth());
	int h = float(texture->getFrameHeight());

	rect = new Rectangle(pos, w * dimension.getX(), h * dimension.getY());
}

SceneObject::~SceneObject()
{
	delete rect;
}

void SceneObject::render() const {
	texture->render(rect->to_sdl());
}
