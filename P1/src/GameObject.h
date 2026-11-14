#pragma once

class Game;

class GameObject
{
private:
    Game* game;

protected:
    explicit GameObject(Game* game) : game(game) {}
    Game* getGame() const { return game; }

public:
    virtual ~GameObject() = default;

    virtual void render() const = 0;
    virtual bool update(float delta) = 0;
};
