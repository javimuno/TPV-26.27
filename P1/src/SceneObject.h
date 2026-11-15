#pragma once

#include "GameObject.h"
#include "rectangle.h"

class Texture;

class SceneObject : public GameObject
{
private:
    Rectangle rect;
    Texture* texture;

protected:
    SceneObject(Game* game, const Rectangle& rect, Texture* texture)
        : GameObject(game), rect(rect), texture(texture) {}

    const Rectangle& getRect() const { return rect; }
    Texture* getTexture() const { return texture; }

    void move(const Vector2D<>& displacement)
    {
        rect.move(displacement);
    }

public:
    virtual bool hit(const Rectangle&) { return false; }
    virtual void save(std::ostream& out) const {
        out << rect.getTopLeftCorner().getX() << ' '
            << rect.getTopLeftCorner().getY() << ' '
            << rect.getWidth() << ' '
            << rect.getHeight();
    }
};
