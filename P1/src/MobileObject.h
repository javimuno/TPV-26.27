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

public:
    bool update(float delta) override
    {        
        move(velocity * delta);
        return true;
    }
};
