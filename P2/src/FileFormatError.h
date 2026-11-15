#pragma once

#include "GameError.h"
#include <string>
#include <cstddef>

class FileFormatError : public GameError {
public:
    FileFormatError(
        const std::string& filename,
        size_t line,
        const std::string& message
    );
};
