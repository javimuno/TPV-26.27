#pragma once

#include "SceneObject.h"

class MobileObject : public SceneObject
{
private:
    Vector2D<> velocity;

protected:
    MobileObject(Game* game, const Rectangle& rect,
        const Vector2D<>& velocity, Texture* texture)
        : SceneObject(game, rect, texture), velocity(velocity) {}

    //constructor de cargar
    MobileObject(Game* game, std::istream& in, Texture* texture)
        : SceneObject(game, in, texture), velocity(readVelocity(in)) {}

    void reverseHorizontalDirection()
    {
        velocity = Vector2D<>(-velocity.getX(), velocity.getY());
    }

    void reverseVerticalDirection()
    {
        velocity = Vector2D<>(velocity.getX(), -velocity.getY());
    }
    void setVelocity(const Vector2D<>& newVelocity) 
    {
        velocity = newVelocity;
    }
    bool movingLeft() const 
    {
        return velocity.getX() < 0.0f;
    }
    //velocidad del objeto movil
    static Vector2D<> readVelocity(std::istream& in) {
        float x, y;

        if (!(in >> x >> y)) {
            throw GameError("Velocidad no valida");
        }
        return Vector2D<>(x, y);
    }

public:
    bool update(float delta) override
    {        
        move(velocity * delta);
        return true;
    }
    //guarda primero el rectangulo de Sceneobject y luego la velocidad de este
    void save(std::ostream& out) const override
    {
        SceneObject::save(out);
        out << ' ' << velocity.getX() << ' ' << velocity.getY();
    }
};
