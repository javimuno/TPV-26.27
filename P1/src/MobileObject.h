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

    
    MobileObject(Game* game, std::istream& in, Texture* texture)//constructor de cargar, primero el rectangulo y luego la velocidad
        : SceneObject(game, in, texture), velocity(readVelocity(in)) {}


    //para dar el cambio de sentido a las mariposas cuando llegan a los bordes
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

    
    static Vector2D<> readVelocity(std::istream& in) {//lee las dos componente de la velocidad al cargar
        float x, y;

        if (!(in >> x >> y)) {
            throw GameError("Velocidad no valida");
        }
        return Vector2D<>(x, y);
    }

public:
    bool update(float delta) override //desplaza el objeto aplicando velocidad * tiempo (solo aplica el movimiento)
    {        
        move(velocity * delta);
        return true;
    }
    
    void save(std::ostream& out) const override //guarda primero el rectangulo de Sceneobject y luego la velocidad de este
    {
        SceneObject::save(out);
        out << ' ' << velocity.getX() << ' ' << velocity.getY();
    }
};
