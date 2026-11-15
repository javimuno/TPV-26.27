#pragma once

#include <SDL3/SDL.h>
#include "GameObject.h"

class InfoBar: public GameObject {
private: 
    void renderArrows() const;
    void renderScore() const;

public:
    InfoBar(Game* g);

    bool update(float dt) override;
    void render() const override;
};

