#pragma once

#include <SDL3/SDL_error.h>
#include <stdexcept>
#include <string>

class GameError : public std::logic_error { //tipo bas de errores del juego
public:
    using std::logic_error::logic_error;
};

class SDLError : public GameError { //añade el mensade de SDL a la opearcion que fallo
public:
    explicit SDLError(const std::string& operation)
        : GameError(operation + ": " + SDL_GetError()) {}
};

class FileNotFoundError : public GameError { //indica que archivo falta
public:
    explicit FileNotFoundError(const std::string& filename)
        : GameError("No se encuentra el archivo: " + filename) {}
};

class FileFormatError : public GameError { //incluye archivo, linea y motivo del error
public:
    FileFormatError(const std::string& filename, int lineNumber,
        const std::string& message)
        : GameError(filename + ":" + std::to_string(lineNumber) + ": " + message)
    {}
};
