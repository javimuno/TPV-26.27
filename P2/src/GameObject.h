#pragma once

class Game;
class GameObject {
public:
    virtual ~GameObject() = default;

    virtual bool update(float dt) = 0;
    virtual void render() const = 0;

protected:
    Game* game;

    GameObject(Game* game) : game(game) {}
};
