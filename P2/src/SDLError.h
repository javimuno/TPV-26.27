#pragma once

#include "GameError.h"

class SDLError : public GameError {
public:
    explicit SDLError(const std::string& message);
};

