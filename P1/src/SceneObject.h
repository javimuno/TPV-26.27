#pragma once

#include "GameObject.h"
#include "rectangle.h"
#include "GameError.h"

#include <istream>
#include <cmath>


class Texture;

class SceneObject : public GameObject
{
private:
    Rectangle rect;
    Texture* texture;

protected:
    SceneObject(Game* game, const Rectangle& rect, Texture* texture)
        : GameObject(game), rect(rect), texture(texture) {}

    SceneObject(Game* game, std::istream& in, Texture* texture)
        : SceneObject(game, readRectangle(in), texture) {}

    const Rectangle& getRect() const { return rect; }
    Texture* getTexture() const { return texture; }

    void move(const Vector2D<>& displacement)
    {
        rect.move(displacement);
    }

    //carga rectangulo del objeto
    static Rectangle readRectangle(std::istream& in) {
        float x, y, width, height;

        if (!(in >> x >> y >> width >> height)) {
            throw GameError("Rectangulo con algun parametro inavlido");
        }

        return Rectangle({ x,y }, width, height);
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
