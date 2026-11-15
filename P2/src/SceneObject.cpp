#include "SceneObject.h"
#include "FileFormatError.h"

SceneObject::SceneObject(Game* game, Texture* tex, const Point2D<float>& pos, const Vector2D<float>& dim)
	: GameObject(game), texture(tex), position(pos), dimension(dim)
{
	int w = float(texture->getFrameWidth());
	int h = float(texture->getFrameHeight());

	rect = new Rectangle(pos, w * dimension.getX(), h * dimension.getY());
}

SceneObject::SceneObject(std::istream& is, Game* game, Texture* tex)
    : GameObject(game), texture(tex), position(), dimension(), rect(nullptr)
{
    float x, y, dimX, dimY;

    if (!(is >> x >> y >> dimX >> dimY))
        throw FileFormatError(
			"saved.txt",
			0,
			"Error al leer los datos de SceneObject"
		);

    position = Point2D<float>(x, y);
    dimension = Vector2D<float>(dimX, dimY);

    int w = texture->getFrameWidth();
    int h = texture->getFrameHeight();

    rect = new Rectangle(
        position,
        w * dimension.getX(),
        h * dimension.getY()
    );
}

SceneObject::~SceneObject()
{
	delete rect;
}

void SceneObject::render() const {
	texture->render(rect->to_sdl());
}

void SceneObject::save(std::ostream& os) const
{
	os << rect->getTopLeftCorner().getX() << " "
		<< rect->getTopLeftCorner().getY() << " ";

	// Guardamos sus dimensiones (escala)
	os << dimension.getX() << " "
		<< dimension.getY() << " ";
}
