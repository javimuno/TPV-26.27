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

    SceneObject(Game* game, std::istream& in, Texture* texture) //recupera rectangulo y textura recibida
        : SceneObject(game, readRectangle(in), texture) {}

    const Rectangle& getRect() const { return rect; }
    Texture* getTexture() const { return texture; }

    void move(const Vector2D<>& displacement)
    {
        rect.move(displacement);
    }

    
    static Rectangle readRectangle(std::istream& in) {//carga rectangulo del objeto, lee posicion y valida dimensiones
        float x, y, width, height;

        if (!(in >> x >> y >> width >> height) || width <= 0.0f || height <= 0.0f) {
            throw GameError("Rectangulo con algun parametro no valido");
        }

        return Rectangle({ x,y }, width, height);
    }

public:
    virtual bool hit(const Rectangle&) { return false; } //por defecto el objeto no recibe impactos
    virtual void save(std::ostream& out) const { //escribe posicon y tamaño del objeto
        out << rect.getTopLeftCorner().getX() << ' '
            << rect.getTopLeftCorner().getY() << ' '
            << rect.getWidth() << ' '
            << rect.getHeight();
    }
};
