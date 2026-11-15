#include "FileNotFoundError.h"

FileNotFoundError::FileNotFoundError(const std::string& filename)
    : GameError("No se pudo abrir el archivo: " + filename)
{
}
