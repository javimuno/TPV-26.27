#pragma once

#include <stdexcept>

class GameError : public std::logic_error {
public:
    using std::logic_error::logic_error;
};
