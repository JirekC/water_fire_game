#pragma once

#include <ostream>

enum class Color { none, red, blue, green };

inline std::ostream& operator<<(std::ostream& os, Color c) {
    switch (c) {
        case Color::none:  return os << "none";
        case Color::red:   return os << "red";
        case Color::blue:  return os << "blue";
        case Color::green: return os << "green";
    }
    return os << "?";
}