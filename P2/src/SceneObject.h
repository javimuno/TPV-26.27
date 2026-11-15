#pragma once
#include "GameObject.h"
#include "texture.h"
#include "vector2D.h"
#include "rectangle.h"
#include <iosfwd>

class Rectangle;
class Game;

class SceneObject : public GameObject {
public:
    SceneObject(Game* game, Texture* tex, const Point2D<float>& pos, const Vector2D<float>& dim);

    SceneObject(std::istream& is, Game* game, Texture* tex);

    virtual ~SceneObject() override;

    virtual void render() const override;

    virtual bool hit(const Rectangle& other) = 0;

    virtual bool canBeHit() const { return false; };

    virtual void save(std::ostream& os) const;

protected:
    Texture* texture = nullptr;
    Point2D<float> position;
    Vector2D<float> dimension;

    Rectangle* rect = nullptr;
};
