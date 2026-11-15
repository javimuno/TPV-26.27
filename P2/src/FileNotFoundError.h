#pragma once

#include "GameError.h"

class FileNotFoundError : public GameError {
public:
    explicit FileNotFoundError(const std::string& filename);
};
