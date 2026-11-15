#include "FileFormatError.h"

FileFormatError::FileFormatError(
    const std::string& filename,
    size_t line,
    const std::string& message
)
    : GameError(
        "Error en el archivo " + filename +
        ", linea " + std::to_string(line) +
        ": " + message
    ) {}
